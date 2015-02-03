//tic27@pitt.edu Tianyang Chen
//CS449 Proj1 ID3 tag editor
//Usage: 
//-help Display this usage information.
//-title [arg] Modify title of the mp3 file.
//-artist [arg] Modify the artist.
//-album [arg] Modify the album.
//-comment [arg] Modify the comment.
//-track [arg] Modify the track.
//2/1/2015
#include <stdio.h>
#include <stdlib.h>
struct song_tag{
	char tag_tag[3];char title[30]; char artist[30]; char album[30]; char year[4]; 
	char comment[28]; char zero_pad[1]; char track_num[1]; char genre[1];
};
const char* program_name;
int readTag(FILE* mp3_file,struct song_tag* tag){//read tags to a struct
	fseek(mp3_file,-128,SEEK_END);
	fread(tag,1,128,mp3_file);
	if(strncmp(tag,"TAG",3)){
		printf("This file doesn\'t have a TAG.\n");
		return 0;//doesn't have a tag
	}
	return 1;//has a tag
}
void printTag(struct song_tag* tag){//print the tag
	char buffer[31]={0};//temp array for printing non-null terminated strings
	strncpy(buffer,tag->title,30);
	printf("Title: %s\n",buffer);strncpy(buffer,tag->artist,30);
	printf("Artist: %s\n",buffer);strncpy(buffer,tag->album,30);
	printf("Album: %s\n",buffer);strncpy(buffer,tag->year,4);
	printf("Year: %s\n",buffer);strncpy(buffer,tag->comment,28);
	printf("Comments: %s\n",buffer);
	printf("Track number is %d\n",*tag->track_num);
	printf("Genre is %d\n",*tag->genre);
}
void appendTag(FILE* mp3_file,struct song_tag* tag){// append a tag to the end
	fwrite(tag,128,1,mp3_file);
	fclose(mp3_file);
	return;
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
	" -help Display this usage information.\n"
	" -title [arg] Modify title of the mp3 file.\n"
	" -artist [arg] Modify the artist.\n"
	" -album [arg] Modify the album.\n"
	" -comment [arg] Modify the comment.\n"
	" -track [arg] Modify the track.\n");
	exit (exit_code);
}

int main(int argc, char * argv[]){
	struct song_tag tag; FILE *mp3_file; int has_tag=0; int arg_index;
	program_name=argv[0];
	if(argc==1){print_usage(stderr,1);return -1;}//if there is no options
	mp3_file = fopen(argv[1],"r");//read only
	if(mp3_file==NULL){
		printf("no such file: %s\n",argv[1]);
		//print_usage(stderr,1);
		return -1;
	}
	
	
	has_tag=readTag(mp3_file,&tag);
	fclose(mp3_file);
	
	if(argc==2){//only print tag if there is only one arg
		if(has_tag){printTag(&tag);return 0;}
		else{printf("There is no tag to print\n");return -1;}
	}

	if(argc%2!=0){printf("Missing arguments\n");print_usage(stderr,1);}
	
	struct song_tag newtag;
	newtag.tag_tag[0]='T';newtag.tag_tag[1]='A';newtag.tag_tag[2]='G';
	memset(newtag.title,0,30);
	memset(newtag.year,0,4);
	memset(newtag.artist,0,30);
	memset(newtag.album,0,30);
	memset(newtag.comment,0,28);
	newtag.zero_pad[0]=0;
	newtag.track_num[0]=0;
	newtag.genre[0]=0;
	
	if(!has_tag){//if the file doesnt have a tag, reopen it and append a new tag
		mp3_file=fopen(argv[1],"ab");
	}
	else{
		mp3_file=fopen(argv[1],"r+b");//modify a tag
	}
	
	for(arg_index=2;arg_index<argc;arg_index++)
	{
		if(!strncmp("-title",argv[arg_index],6)){//-title is detected
				printf("title\n");
				strncpy(newtag.title,argv[++arg_index],30);
				if(has_tag){
					modifyTag(mp3_file,-128+3,newtag.title,30);
				}				
		}	
		else if(!strncmp("-artist",argv[arg_index],7)){//-artist is detected
				printf("artist\n");
				strncpy(newtag.artist,argv[++arg_index],30);
				if(has_tag){
					modifyTag(mp3_file,-128+33,newtag.artist,30);
				}
		}
		else if(!strncmp("-album",argv[arg_index],6)){//-album is detected
				printf("album\n");
				strncpy(newtag.album,argv[++arg_index],30);
				if(has_tag){	
					modifyTag(mp3_file,-128+63,newtag.album,30);
				}
		}
		else if(!strncmp("-year",argv[arg_index],5)){//-year is detected
				printf("year\n");
				strncpy(newtag.year,argv[++arg_index],4);
				if(has_tag){
					modifyTag(mp3_file,-128+93,newtag.year,4);
				}
		}
		else if(!strncmp("-comment",argv[arg_index],8)){//-comment is detected
				printf("comment\n");
				strncpy(newtag.comment,argv[++arg_index],28);
				printf("comment is changed into %s\n",newtag.comment);
				if(has_tag){
					modifyTag(mp3_file,-128+97,newtag.comment,28);
				}	
		}
		else if(!strncmp("-track",argv[arg_index],6)){//-track is detected
				
				newtag.track_num[0]=(char)atoi(argv[++arg_index]);
				printf("track has been changed into %d\n",newtag.track_num);
				if(has_tag){
					modifyTag(mp3_file,-128+126,newtag.track_num,1);
				}
		}
		else{
			print_usage(stderr,1);//nothing is recognised
		}
	}
	if(!has_tag){
		appendTag(mp3_file,&newtag);
		printf("new tag has been created!\n");
	}
	else{
		printf("tag has been modified!\n");
		fclose(mp3_file);
	}
	
	
}
	
	
	
	
	
	
	