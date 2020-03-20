// <--- means personal added input
// <=== means did not finish yet, or need to debug

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define CONFIG_LINE 100
#define STR_LINE 5
#define BYTE 8
#define PT 10

typedef struct{             /* page table entry */
  unsigned char presence;   /* could be single bit */
  unsigned short pad;       /* 16 bits *//* padding bit */
  unsigned char pfn;        /* 8 bits */
} PTE;

typedef struct{             /* TLB entry */
  unsigned char valid;      /* could be single bit */
  unsigned short vpn;       /* 16 bits */
  unsigned char pfn;        /* 8 bits */
} TLBE;
 
typedef struct{             /* Memory access count */
  unsigned char valid;      /* could be single bit */
  unsigned char use_vector; /* 8 bits for pseudo-LRU replacement */
  unsigned short vpn;       /* 16 bits */
} CME;

typedef struct{// cfg
  int PF;                   /* Page Table Entry # */
  int TE;                   /* TLB Entry # */
  int UP;                   /* use period vector bits # */
} CFG;// my structure

PTE pte[65536];// Page table
TLBE *tlbe;// TLB
CME *cme;// core map
CFG cfg;// Three data that is needed 

bool verbose = 0;
bool error = 0;

int read_cfg();
void print_stats();

void tlb_update(unsigned short, unsigned char);
void pt_update(unsigned short, unsigned char);
void cm_update(unsigned short, unsigned char);
unsigned access = 0;
unsigned tlb_miss = 0;
unsigned page_fault = 0;
unsigned FIFO_index = 0;

int main(int argc, char* argv[]) {
/* Open the paging.cfg */
  if(read_cfg()){
    printf("There are not paging.cfg found in the current directry!\n");
    return EXIT_FAILURE;
  }
/* Open the paging.cfg */

/* Saving the Virtual Address, and compare with the TLB Entries */  

  tlbe = (TLBE *) calloc(cfg.TE, sizeof(TLBE));// allocate and initialize the TLB entry
  cme = (CME *) calloc(cfg.UP, sizeof(CME));// allocate and initialize the core map entry
  /*Initialize the variable that are to use*/

  unsigned va = 0;
  unsigned pa = 0;
  
  unsigned i;// for loop increment variable
  
  if (argc == 2 && strcmp(argv[1], "-v") == 0) verbose = 1;//verbose output
  
  if (verbose) {
    printf("paging simulation\n");
    printf("  65536 virtual pages in the virtual address space\n");
    printf("  %d physical page frames\n", cfg.PF);
    printf("  %d TLB entries\n", cfg.TE);
    printf("  use vectors in core map are shifted every %d accesses\n", cfg.UP);
    printf("\n");
  }
  
  while ( scanf("%x", &va) != EOF ) {
    /*Error Avoidance*/
    if ((va & 0xf000000) > 0) { 
      error = 1;
      printf("\n");
      print_stats();
      printf("\n");
      continue;
    }
    error = 0;
    /*Error Avoidance*/
    access ++;
    if (verbose) {
      printf("access %d:\n", access);
      printf("  virtual address is              0x%06x\n", va);
    }
    unsigned short addr_vpn = (va & 0xffff00) >> BYTE;
    unsigned char offset = va & 0x0000ff;
 
    /* 1) Check the TLB */
    bool tlb_hit_flag = 0;
    for (i = 0; i < cfg.TE; i ++) {// linear search for TLB
      if (tlbe[i].vpn == addr_vpn && tlbe[i].valid == 1) {
        /*2A) If there is a TLB hit, use the PFN from the TLB entry to
        obtain the physical address. You are done.*/
        tlb_hit_flag = 1;
        pa = tlbe[i].pfn << BYTE;// <--------------------------tlb hit: vfn fetched in tlb
        pa += offset;
        cme[pte[addr_vpn].pfn].use_vector |= 0x1 << (BYTE - 1);
        if (verbose) {
          printf("  tlb hit, physical address is      0x%04x\n",pa);
        }
        break;
      }
    }
    /* 2B) TLB miss, check the page table */
    if (!tlb_hit_flag) {
      if (verbose) {
        printf("  tlb miss\n");
      }
      tlb_miss ++;
      // check the page table 
      if (pte[addr_vpn].presence == 1) {
        // 3A) page hit
        // use the PFN from page table entry to obtain physical address
        pa = pte[addr_vpn].pfn << BYTE;// <------------------page hit: vfn fetched in page
        pa += offset;
        cme[pte[addr_vpn].pfn].use_vector |= 0x1 << (BYTE - 1);
        if (verbose) {
          printf("  page hit, physical address is     0x%04x\n",pa);
        }
        tlb_update(addr_vpn, pte[addr_vpn].pfn);
      } else {
        // 3B) page fault,
        if (verbose) {
          printf("  page fault\n");
        } 
        page_fault ++;
        //find free frame
        bool free_frame_flag = 0;
        for (i = 0; i < cfg.PF; i ++) {
          if (cme[i].valid == 0) {
            // 4A) free frame update core map and page table then calculate the pa
            free_frame_flag = 1;
            pa = i << BYTE;
            pa += offset;
            if (verbose) {
              printf("  unused page frame allocated\n");
              printf("  physical address is               0x%04x\n", pa);
            }
            cm_update(addr_vpn, (unsigned char)i);
            pt_update(addr_vpn, (unsigned char)i);
            tlb_update(addr_vpn, (unsigned char)i);
            break; 
          } 
         
        }
        //4B) no free frame!
        if (!free_frame_flag) {
          if (verbose) {
            printf("  page replacement needed\n");
          }
          unsigned char min_vector = cme[0].use_vector;
          unsigned char min_index = 0;
          for (i = 0; i < cfg.PF; i ++) {
            if (min_vector > cme[i].use_vector) {
              min_vector = cme[i].use_vector;
              min_index = i;
            }
          }
          pa = min_index << BYTE;
          pa += offset; 
          if (verbose) {
            printf("  TLB invalidate of vpn 0x%x\n", cme[min_index].vpn);
            printf("  replace frame %d\n", min_index);
            printf("  physical address is               0x%04x\n", pa);
          }
          // Invalidate if any
          for (i = 0; i < cfg.TE; i ++) {
            if (tlbe[i].valid == 1 && tlbe[i].vpn == cme[min_index].vpn) {
              tlbe[i].valid = 0;
              tlbe[i].vpn = 0;
              tlbe[i].pfn = 0;
              break;
            }
          }
          pte[cme[min_index].vpn].presence = 0;
          pte[cme[min_index].vpn].pfn = 0;
          
          cme[min_index].vpn = 0;
          cme[min_index].use_vector = 0;
          cme[min_index].valid = 0;
          
          cm_update(addr_vpn, min_index);

          pt_update(addr_vpn, min_index);
          
          tlb_update(addr_vpn, min_index);
          
        }
        free_frame_flag = 0;
      }
    }
    tlb_hit_flag = 0;
    
    if (access % cfg.UP == 0) {
      for (i = 0; i < cfg.PF; i ++) {
        cme[i].use_vector >>= 0x1;
      }
      if (verbose) {
        printf("shift use vectors\n");
      }
    }

  }
  if (verbose) printf("\n");
  print_stats();  
  return EXIT_SUCCESS;
}

/*read configuration file*/
  
int read_cfg() {  
  char buf[CONFIG_LINE];// use to store the reading from config file
  char str[STR_LINE];   // use to parse the string and the data
  FILE *fp;             // file pointer to take in the paging.cfg
  //char line[MAXLINE];   // receive stdin
  if ((fp = fopen("paging.cfg","r")) == NULL) {// while cfg do not exists
    fprintf(stderr, "Failed to open config file paging.cfg");
    return EXIT_FAILURE;
  }
  while (! feof(fp)) {// while cfg exists, parse the file with sscanf
    fgets(buf, CONFIG_LINE, fp);
    
    if (strstr(buf, "PF ")) {
      sscanf(buf, "%s%d", str, &cfg.PF);
      //printf("page_frame in config:%d\n", cfg.PF);//<------------------
    }
    if (strstr(buf, "TE ")) {
      sscanf(buf, "%s%d", str, &cfg.TE);
      //printf("TLB_entries in config:%d\n", cfg.TE);//<-----------------
    }
    if (strstr(buf, "UP ")) {
      sscanf(buf, "%s%d", str, &cfg.UP);
      //printf("usage_period in config:%d\n", cfg.UP);//<----------------
    } 
  }
  fclose(fp);
  return EXIT_SUCCESS;
}

/*read configuration file*/

void tlb_update(unsigned short new_vpn, unsigned char new_pfn) {
  unsigned i;
  bool tlb_empty_flag = 0;
  if (verbose) {
    printf("  tlb update of vpn 0x%04x with pfn 0x%02x\n", new_vpn, new_pfn);
  }
  for (i = 0; i < cfg.TE; i ++) {
    if (!tlbe[i].valid) {
      tlb_empty_flag = 1;
      tlbe[i].vpn = new_vpn;
      tlbe[i].pfn = new_pfn;
      tlbe[i].valid = 1;
      break;
    }
  }
  if (!tlb_empty_flag) {// replace
    tlbe[FIFO_index].vpn = new_vpn;
    tlbe[FIFO_index].pfn = new_pfn;
    tlbe[FIFO_index].valid = 1;
    FIFO_index ++;
    FIFO_index %= cfg.TE;
  }
  tlb_empty_flag = 0;
}

void pt_update(unsigned short new_vpn, unsigned char new_pfn) {
  pte[new_vpn].pfn = new_pfn;
  pte[new_vpn].presence = 1;
}
void cm_update(unsigned short new_vpn, unsigned char new_pfn) {
  cme[new_pfn].vpn = new_vpn;
  cme[new_pfn].use_vector = 0x1 << (BYTE - 1);
  cme[new_pfn].valid = 1;
}

void print_stats() {
  printf("statistics\n");
  printf("  accesses    = %d\n", access);
  printf("  tlb misses  = %d\n", tlb_miss);
  printf("  page faults = %d\n", page_fault);
  if (error||verbose) {
    printf("\n");
    unsigned i = 0;
    printf("tlb\n");
    for (i = 0; i < cfg.TE; i ++) {
      printf("  valid = %u, vpn = 0x%04x, pfn = 0x%02x\n", 
      tlbe[i].valid, tlbe[i].vpn, tlbe[i].pfn);
    }
    printf("\n");
    printf("core map table\n");
    for (i = 0; i < cfg.PF; i ++) {
      printf("  pfn = 0x%02x: valid = %u, use vector = 0x%02x, vpn = 0x%04x\n",
      i, cme[i].valid, cme[i].use_vector, cme[i].vpn);
    }
    printf("\n");
    printf("first ten entries of page table\n");
    for (i = 0; i < PT; i ++) {
      printf("  vpn = 0x%04x: presence = %u, pfn = 0x%02x\n",
      i, pte[i].presence, pte[i].pfn);
    }
  }
}
