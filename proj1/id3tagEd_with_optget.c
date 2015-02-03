//tic27@pitt.edu Tianyang Chen
//CS449 Proj1 ID3 tag editor
//Usage: 
// -h --help Display this usage information.
// -t --title [arg] Modify title of the mp3 file.
// -a --artist [arg] Modify the artist.
// -l --album [arg] Modify the album.
// -c --comment [arg] Modify the comment.
// -k --track [arg] Modify the track.
//2/1/2015
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
struct song_tag{
	char title[31]; char artist[31]; char album[31]; char year[5]; 
	char comment[31]; char track_num; char genre;
};
const char* program_name;
int readTag(FILE* mp3_file,struct song_tag* tag){//read tags to a struct
	char buffer[129];
	fseek(mp3_file,-128,SEEK_END);
	fread(buffer,128,1,mp3_file);
	if(strncmp(buffer,"TAG",3)){
		printf("This file doesn\'t have a TAG.\n");
		return 0;
	}
	else{
		strncpy(tag->title,&buffer[3],30);tag->title[30]=0x00;
		strncpy(tag->artist,&buffer[33],30);tag->artist[30]=0x00;
		strncpy(tag->album,&buffer[63],30);tag->album[30]=0x00;
		strncpy(tag->year,&buffer[93],4);tag->year[4]=0x00;
		strncpy(tag->comment,&buffer[97],28);tag->comment[29]=0x00;  
		tag->track_num=buffer[126];    
		tag->genre=buffer[127];
		return 1;
	}
}

void printTag(struct song_tag* tag){//print the tag
	printf("Title: %s\n",tag->title);
	printf("Artist: %s\n",tag->artist);
	printf("Album: %s\n",tag->album);
	printf("Year: %s\n",tag->year);
	printf("Comments: %s\n",tag->comment);
	printf("Track number is %d\n",tag->track_num);
	printf("Genre is %d\n",tag->genre);
}
void write_tag(FILE* mp3_file,struct song_tag* tag, int has_tag){//append a new tag to the file if it doesnt have one
	if(!has_tag){//append a tag at the end of the mp3 file
		fwrite("TAG",1,3,mp3_file);
		fwrite(tag->title,1,30,mp3_file);
		fwrite(tag->artist,1,30,mp3_file);
		fwrite(tag->album,1,30,mp3_file);
		fwrite(tag->year,1,4,mp3_file);
		fwrite(tag->comment,1,28,mp3_file);
		fwrite("",1,1,mp3_file);
		fwrite(&(tag->track_num),1,1,mp3_file);
		fwrite(&(tag->genre),1,1,mp3_file);
	}
	
}
void modifyTag(FILE* mp3_file, int offset, char* content, int size){//modify the current tag
	//printf("modifying\n");
	fseek(mp3_file,offset,SEEK_END);
	fwrite(content,1,size,mp3_file);
	
}
void print_usage (FILE* stream, int exit_code)
{
	fprintf (stream, "Usage: %s options [ inputfile ... ]\n", program_name);
	fprintf (stream,
	" -h --help Display this usage information.\n"
	" -t --title [arg] Modify title of the mp3 file.\n"
	" -a --artist [arg] Modify the artist.\n"
	" -l --album [arg] Modify the album.\n"
	" -c --comment [arg] Modify the comment.\n"
	" -k --track [arg] Modify the track.\n");
	
	exit (exit_code);
}
int main(int argc, char * argv[]){
	unsigned char buffer[129]; struct song_tag tag; FILE *mp3_file;
	char* filename;
	int print_tag=0;
	int modify_tag=0;
	int has_tag=0;
	int next_option;
	
	const char* const short_options="ht:y:a:l:c:k:";
	const struct option long_options[] = {
		{ "help",   0, NULL, 'h' },
		{ "title",  1, NULL, 't' },
		{ "year",   1, NULL, 'y' },
		{ "artist", 1, NULL, 'a' },
		{ "album",  1, NULL, 'l' },
		{ "comment",1, NULL, 'c' },
		{ "track",  1, NULL, 'k' },
	{  NULL,    0, NULL,  0  } };
	
	filename=argv[1];
	program_name=argv[0];
	
	buffer[128]=0x00;
	mp3_file = fopen(argv[1],"r+");
	if(mp3_file==NULL){
		printf("no such file: %s\n",argv[1]);
		print_usage(stderr,1);
		return -1;
	}
	if(argc==1){print_usage(stderr,1);}
	has_tag=readTag(mp3_file,&tag);
	if(argc==2){//only print tag if there is only one arg
		if(has_tag){printTag(&tag);}	
	}
	else{//modify the tag or create a tag
		struct song_tag newtag;
		memset(newtag.title,0,30);
		memset(newtag.year,0,4);
		memset(newtag.artist,0,30);
		memset(newtag.album,0,30);
		memset(newtag.comment,0,28);
		newtag.track_num=0;
		newtag.genre=0;
		if(!has_tag){//if the file doesnt have a tag, reopen it and append a new tag
			fclose(mp3_file);
			mp3_file=fopen(argv[1],"a");
		}
		do{
			next_option=getopt_long(argc,argv,short_options,
			long_options,NULL);
			switch(next_option){
				case 'h':
				print_usage(stdout,0);
				break;
				case 't':
				printf("You want to change title into %s\n",optarg);
				strncpy(newtag.title,optarg,30);//newtag.title[29]=0x00;
				if(has_tag){
					modifyTag(mp3_file,-128+3,newtag.title,30);
				}
				break;
				case 'a':
				printf("You want to change artist into %s\n",optarg);
				strncpy(newtag.artist,optarg,30);//newtag.artist[29]=0x00;
				if(has_tag){
					modifyTag(mp3_file,-128+33,newtag.artist,30);
				}
				break;
				case 'l':
				printf("You want to change album into %s\n",optarg);
				strncpy(newtag.album,optarg,30);//newtag.album[29]=0x00;
				if(has_tag){	
					modifyTag(mp3_file,-128+63,newtag.album,30);
				}
				break;
				case 'y':
       	        printf("You want to change year into %s\n",optarg);
				if(strlen(optarg)>4){
					printf("your input is longer than 4. Some info may be lost\n");
				}
       	        strncpy(newtag.year,optarg,4);//newtag.album[4]=0x00;
				if(has_tag){
					modifyTag(mp3_file,-128+93,newtag.year,4);
				}
				break;
				case 'c':
                printf("You want to change comment into %s\n",optarg);
                strncpy(newtag.comment,optarg,28);//newtag.comment[29]=0x00;
				if(!has_tag){
					modifyTag(mp3_file,-128+97,newtag.comment,28);
				}	
				break;
				case 'k':
                printf("You want to change track number into %s\n",optarg);
                newtag.track_num=atoi(optarg);
				if(has_tag){
					modifyTag(mp3_file,-128+126,&newtag.track_num,1);
				}
				break;
				
				case '?':
				print_usage(stderr,1);
				
				break;	
				case -1:
				break;
				default:
				abort();
			}
			
		}while(next_option!=-1);
		
		write_tag(mp3_file,&newtag,has_tag);//append a new tag
		fclose(mp3_file);
		printf("done writing\n");
	}
	
	
	
	
	
	
	return 0;
	
	
	
}
