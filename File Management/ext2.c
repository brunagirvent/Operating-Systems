#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#include "ext2.h"

const unsigned int ext2_bs[] = {
	1024,
	2048,
	4096
};

void ext2_print_sb(struct ext2_sb *sb)
{
	time_t t;

	fprintf(stdout,"I-nodes: %u\n",sb->s_inodes_count);
	fprintf(stdout,"Blocks: %u\tReserved Blocks: %u\n",
		sb->s_blocks_count,sb->s_r_blocks_count);
	fprintf(stdout,"Free Blocks: %u\tFree i-nodes: %u\n",
		sb->s_free_blocks_count,sb->s_free_inodes_count);
	fprintf(stdout,"First Data Block: %u\n",sb->s_first_data_block);
	fprintf(stdout,"Block Size: %u\tFragment Size: %u\n",
		ext2_bs[sb->s_log_block_size],sb->s_log_frag_size);
	fprintf(stdout,"Per Group: %u blocks\t%u fragments\t%u i-nodes\n",
		sb->s_blocks_per_group,sb->s_frags_per_group,sb->s_inodes_per_group);
	t = sb->s_mtime;
	fprintf(stdout,"Last Mounted: %s",ctime(&t));
	t = sb->s_wtime;
	fprintf(stdout,"Last Written: %s",ctime(&t));
	fprintf(stdout,"Mounted %hu times\t %hu mounts before check\n",
		sb->s_mnt_count,sb->s_max_mnt_count);
	fprintf(stdout,"Magic: %hx\tState: %hx\tErrors: %hx\n",
		sb->s_magic,sb->s_state,sb->s_errors);
	t = sb->s_lastcheck;
	fprintf(stdout,"Last checked: %s",ctime(&t));
	fprintf(stdout,"Check every %u days\n",sb->s_checkinterval/3600/24);
}

void ext2_print_bg(struct ext2_bg *bg)
{
	fprintf(stdout,"Block bitmap at %u\n",bg->bg_block_bitmap);
	fprintf(stdout,"I-node bitmap at %u\n",bg->bg_inode_bitmap);
	fprintf(stdout,"I-node table at %u\n",bg->bg_inode_table);
	fprintf(stdout,"Free Blocks: %hu\tFree i-nodes: %hu\n",
		bg->bg_free_blocks_count,bg->bg_free_inodes_count);
	fprintf(stdout,"Directory i-nodes: %hu\n",bg->bg_used_dirs_count);
}



int main (int argc, char *argv[]) {
	int fdO,fdD,nhijos,i=0,j,pos;
	char *fichero="fs.img";
	char filename[256];

	struct ext2_sb sb;
	struct ext2_bg bg;
	struct ext2_inode inode;
	struct ext2_dentry dentry;

    fdO=open(fichero,O_RDONLY);

	/*Leer superbloque*/
	lseek(fdO,ext2_bs[0],SEEK_SET); //Posicionamos en el primer superbloque
									// (ext2bs_0=1024 definido arriba)
	read(fdO,&sb,sizeof(struct ext2_sb));
	ext2_print_sb(&sb);
	

	/*Crear hijos*/
	nhijos=sb.s_blocks_count/sb.s_blocks_per_group; //numero bloques=numero hijos
	if (sb.s_blocks_count%sb.s_blocks_per_group!=0) nhijos++;

	fdD=open("ext2.txt",O_RDWR|O_CREAT|O_TRUNC,0600);

	while(i<nhijos) {
		if(fork()==0) { //hijo

			/*lectura descriptor grupo */
			read(fdO,&bg,sizeof(struct ext2_bg));  
			printf("\nGroup %d\n",i);
			ext2_print_bg(&bg);  
 
			close(1); //Redireccion salida estandar a fichero destino
			dup(fdD);

			for(j=0;j<sb.s_inodes_per_group;j++){
				/*lectura inodo*/
				lseek(fdO,bg.bg_inode_table*ext2_bs[0]+j*sizeof(struct ext2_inode),SEEK_SET); //posicionar
				read(fdO,&inode,sizeof(struct ext2_inode)); //lectura inodo

				if (inode.i_mode&((S_IFMT&S_IFDIR))) { //si directorio
					/*lectura directorio*/
					lseek(fdO,inode.i_block[0]*ext2_bs[0],SEEK_SET);
					read(fdO,&dentry,sizeof(struct ext2_dentry));
					
					while(dentry.inode>0 && dentry.inode<=sb.s_inodes_count) {
						/*lectura nombre fichero*/
						read(fdO,filename,dentry.name_len); //lectura nombre fichero
						filename[dentry.name_len]='\0';
						printf("Group: %d Inode: %d, Index: %d Name: %s\n",i,dentry.inode,j,filename);
						lseek(fdO,dentry.rec_len - dentry.name_len - sizeof(struct ext2_dentry),SEEK_CUR);
						read(fdO,&dentry,sizeof(struct ext2_dentry)); //lectura directorio
					}
				}
			}//for	
			exit(0);
		}//if

		wait(NULL);
		i++;
	}//while

	close(fdD);
	close(fdO);
	return (0);

}

