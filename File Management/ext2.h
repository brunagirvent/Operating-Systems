#ifndef EXT2_H
#define EXT2_H

/* Ext2 SuperBlock */
struct ext2_sb {
	unsigned int s_inodes_count;
	unsigned int s_blocks_count;
	unsigned int s_r_blocks_count;
	unsigned int s_free_blocks_count;
	unsigned int s_free_inodes_count;
	unsigned int s_first_data_block;
	unsigned int s_log_block_size;
	signed int s_log_frag_size;
	unsigned int s_blocks_per_group;
	unsigned int s_frags_per_group;
	unsigned int s_inodes_per_group;
	unsigned int s_mtime;
	unsigned int s_wtime;
	unsigned short s_mnt_count;
	signed short s_max_mnt_count;
	unsigned short s_magic;
	unsigned short s_state;
	unsigned short s_errors;
	unsigned short s_pad;
	unsigned int s_lastcheck;
	unsigned int s_checkinterval;
	unsigned int s_creator_os;
	unsigned int s_rev_level;
	unsigned int s_reserved[236];
};

struct ext2_bg {
	unsigned int bg_block_bitmap;
	unsigned int bg_inode_bitmap;
	unsigned int bg_inode_table;
	unsigned short bg_free_blocks_count;
	unsigned short bg_free_inodes_count;
	unsigned short bg_used_dirs_count;
	unsigned short bg_pad;
	unsigned int bg_reserved[3];
};

struct ext2_inode {
	unsigned short i_mode;
	unsigned short i_uid;
	unsigned int i_size;
	unsigned int i_atime;
	unsigned int i_ctime;
	unsigned int i_mtime;
	unsigned int i_dtime;
	unsigned short i_gid;
	unsigned short i_links_count;
	unsigned int i_blocks;
	unsigned int i_flags;
	unsigned int i_reserved1;
	unsigned int i_block[15];
	unsigned int i_version;
	unsigned int i_file_acl;
	unsigned int i_dir_acl;
	unsigned int i_faddr;
	unsigned char i_frag;
	unsigned char i_fsize;
	unsigned short i_pad;
	unsigned int i_reserved2[2];
};

struct ext2_dentry {
	unsigned int inode;
	unsigned short rec_len;
	unsigned char name_len;
	unsigned char file_type;
	
};

extern const unsigned int ext2_bs[];

/* Exported Functions */
void ext2_print_sb(struct ext2_sb *);
void ext2_print_bg(struct ext2_bg *);

#endif

