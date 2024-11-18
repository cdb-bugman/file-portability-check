#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>

#define NA 0
#define BLACKLIST 0
#define WHITELIST 1

struct standard_definition {
  char*      name;                      /* Name of standard. */
  char*      charlist;                  /* Characters that comprise a whitelist or blacklist. */
  u_int64_t  size_limit;  		/* How large, in bytes, a file may be. */
  u_int64_t  max_file_count;	        /* How many files a directory may hold.  */
  u_int64_t  component_len;	        /* How many characters long a path component may be.  */
  u_int64_t  path_len;		        /* How many characters long a path may be.  */
  bool       are_duplicates_allowed;    /* If this standard allows for duplicate file names. */
  bool       are_symlinks_allowed;      /* If this standard allows for symbolic links.*/
  bool       are_hardlinks_allowed;     /* If this standard allows for hardlinks. */
  bool       charlist_type;             /* WHITELIST or BLACKLIST? */
};


struct standard_definition GLOBAL_STANDARDS[] =
  {
    {"POSIX", "/abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.-", NA, NA, 17, 255, true, true, true, WHITELIST },
    {"FAT32", NA, 4294967296, NA, NA, 32767, NA, false, false, NA },
    {"AdvFS", NA, 17592186044416, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"APFS", NA, 9223372036854775808, 9223372036854775808, 255, NA, false, true, true, BLACKLIST}, 
    {"Bcachefs", NA, 18446744073709551615, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"BeeGFS", NA, 18446744073709551615, NA, 255, NA, true, true, false, BLACKLIST}, 
    {"BFS", NA, 12288, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"Bluestore", NA, 1099000000000, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"Btrfs", NA, 18446744073709551615, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"CBMDOS", NA, 16770000, NA, 16, NA, true, true, true, BLACKLIST}, 
    {"CP/M", "<>., ;:=?*[]", 33550000, NA, 8, NA, true, true, true, BLACKLIST}, 
    {"DECTAPE", "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890", 369280, NA, 11, 15, false, false, false, WHITELIST}, 
    {"ElektronikaBK", NA, 65530, NA, 16, NA, true, true, true, BLACKLIST}, 
    {"exFAT", "\"*/:<>?\\|", 18446744073709551615, NA, 255, 32760, false, false, false, BLACKLIST}, 
    {"ext", NA, 2147483648, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"ext2", NA, 17179869184, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"ext3", NA, 17179869184, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"ext4", NA, 17179869184, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"F2FS", NA, 4329000000000, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"FAT", NA, 8000000, NA, 9, NA, true, true, true, BLACKLIST}, 
    {"FAT12", "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!#$%&'()-@^_`{}~", 33550000, NA, 11, NA, false, false, false, WHITELIST}, 
    {"FAT16", "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!#$%&'()-@^_`{}~", 2147483648, NA, 11, NA, false, false, false, WHITELIST}, 
    {"FAT16B", "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!#$%&'()-@^_`{}~", 2147483648, NA, 11, NA, false, false, false, WHITELIST}, 
    {"FAT16X", "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!#$%&'()-@^_`{}~", 4294967296, NA, 11, NA, false, false, false, WHITELIST}, 
    {"FAT32", "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!#$%&'()-@^_`{}~", 4294967296, NA, 11, 32760, false, false, false, WHITELIST}, 
    {"FAT32X", "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789!#$%&'()-@^_`{}~", 4294967296, NA, 11, 32760, false, false, false, WHITELIST}, 
    {"FATX", NA, 2147483648, NA, 42, NA, true, true, true, BLACKLIST}, 
    {"FFS", NA, 4294967296, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"GECDOS_FILING_EXTENDED", "ABCDEFGHIKLMNOPQRSTUVWXYZ1234567890", 131702, NA, 8, NA, true, true, true, WHITELIST}, 
    {"GEMDOS", "ABCDEFGHIKLMNOPQRSTUVWXYZabcdefghiklmnopqrstuvwxyz1234567890!@#$%^&()+-=~`;'\"<>|[]()_", NA, NA, 11, NA, true, true, true, WHITELIST}, 
    {"GFS2", NA, 109951162777600, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"GFS", NA, 2199023255552, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"GPFS", NA, 10376293541461622784, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"HAMMER", NA, NA, NA, 1023, NA, true, true, true, BLACKLIST}, 
    {"HPFS", NA, 2147483648, NA, 255, NA, false, false, false, BLACKLIST}, 
    {"IBM_SFS", NA, NA, NA, 16, NA, true, true, true, BLACKLIST}, 
    {"ISO_9660_1988_LEVEL1", "ABCDEFGHIKLMNOPQRSTUVWXYZ1234567890_.", 4294967296, NA, 11, 180, false, false, false, WHITELIST}, 
    {"ISO_9660_1988_LEVEL2", "ABCDEFGHIKLMNOPQRSTUVWXYZ1234567890_.", 8796093022208, NA, 180, 180, false, false, false, WHITELIST}, 
    {"JFS", NA, 4503599627370496, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"JFS1", NA, 9223372036854775808, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"Joilet", "*/\\:;?", 4294967296, NA, 64, NA, false, false, false, BLACKLIST}, 
    {"Level-D", "ABCDEFGHIKLMNOPQRSTUVWXYZ1234567890", NA, NA, 9, 32, false, false, false, BLACKLIST}, 
    {"Lustre", NA, 18446744073709551615, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"MFS", NA, 268435456, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"MicroDOS", NA, 16777216, NA, 14, NA, true, true, true, BLACKLIST}, 
    {"Minix_V1_FS", NA, 26895974, NA, 14, NA, true, true, true, BLACKLIST}, 
    {"Minix_V2_FS", NA, 2147483648, NA, 14, NA, true, true, true, BLACKLIST}, 
    {"Minix_V3_FS", NA, 2147483648, NA, 30, NA, true, true, true, BLACKLIST}, 
    {"NILFS", NA, 9223372036854775808, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"NOVA", NA, 18446744073709551615, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"NSS", NA, 8796093022208, NA, 256, NA, true, true, true, BLACKLIST}, 
    {"NTFS", "/\\:*?<>|", 17592186044416, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"NWFS", NA, 4294967296, NA, 80, NA, true, true, true, BLACKLIST}, 
    {"OCFS", NA, 8796093022208, NA, 255, NA, true, true, false, BLACKLIST}, 
    {"OCFS2", NA, 4503599627370496, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"ODS-5", NA, 1099511627776, NA, 236, 4096, true, true, true, BLACKLIST}, 
    {"QFS", NA, 18446744073709551615, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"ReFS", "/\\:*?<>|", 18446744073709551615, NA, 255, 32767, true, true, true, BLACKLIST}, 
    {"ReiserFS", NA, 8796093022208, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"Reiser4", NA, 8796093022208, NA, 3976, NA, true, true, true, BLACKLIST}, 
    {"Rock_Ridge", NA, 4294967296, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"RT-11", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789$", 33554432, NA, 9, NA, false, false, false, WHITELIST}, 
    {"SquashFS", NA, 18446744073709551615, NA, 256, NA, true, true, true, BLACKLIST}, 
    {"UDF", NA, 18446744073709551615, NA, 255, 1023, true, true, true, BLACKLIST}, 
    {"UFS1", NA, 17179869184, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"UFS2", NA, 549755813888, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"V6FS", NA, 16777216, NA, 14, NA, true, false, true, BLACKLIST}, 
    {"V7FS", NA, 1073741824, NA, 14, NA, true, false, true, BLACKLIST}, 
    {"VMFS2", NA, 4398046511104, NA, 128, 2048, true, true, true, BLACKLIST}, 
    {"VMFS3", NA, 2199023255552, NA, 128, 2048, true, true, true, BLACKLIST}, 
    {"VxFS", NA, 18446744073709551615, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"XFS", NA, 9223372036854775808, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"Xiafs", NA, 67108864, NA, 248, NA, true, true, true, BLACKLIST}, 
    {"ZFS", NA, 18446744073709551615, NA, 255, NA, true, true, true, BLACKLIST}, 
    {"TESTING", "/abcdefgh", 1, 1, 1, 1, false, false, false, BLACKLIST},
    {"XOPEN", NA, NA, NA, 255, 1023, true, true, true, WHITELIST},
    {NULL},
  };

