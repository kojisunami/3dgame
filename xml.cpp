
#include "xml.h"

using namespace std;

int Xml_Tree::bracket_start(int off){
	for(int i=off;i<size;i++)
		if(buf[i] == '<')
			return i;
	return -1;
}

int Xml_Tree::bracket_end(int off){
	for(int i=off;i<size;i++)
		if(buf[i] == '>')
			return i;
	return -1;
}


int str_match(char *s0, char *s1, int len){
	//int c = min( min(len, (int)strlen(s0)), (int)strlen(s1));
	if(strlen(s0) < strlen(s1))
		return -1;
	int c = min( len, (int)strlen(s1));
	for(int i =0;i<c;i++){
		if(s0[i] != s1[i])
			return -1;	
	}
	return 1;
}


// this function should only be called for root branch
int Xml_Tree::type(){

	// search for header information <!DOCTYPE>
	if( str_match(xml_list[0].name,(char *)"!DOCTYPE", 8) == 1){
		//printf("header found\n");
		xml_type = (char*) malloc(sizeof(char)*xml_list[0].arg_len[1]);
		xml_type = xml_list[0].arg[1];
		printf("xml type : %s\n", xml_type);
		xml_list.erase(xml_list.begin());
	}

	return 0;
}

void Xml_Bracket::str_spacer(char *str, int len){
	
	num_arg=0;
	for(int i=0;i<256;i++)
		arg_len[i] = 0;
	int flag=0;

	// find num of args 
	for(int i =0;i<len;i++){
		if( (str[i] == 0x0a) || (str[i] == 0x00) || (i == len-1)){
			if(flag ==1)
				num_arg++;
			break;
		}
		else if( (str[i] != ' ') && (str[i] != '\t')){
			flag = 1;
		}
		else if(flag == 1){
			flag = 0;
			num_arg++;
		}
	}

	//arg = (char**) malloc(sizeof(char)*num_arg);
	arg = (char**) malloc(256);
	for(int i =0;i<num_arg;i++)
		arg[i]=(char*)malloc(sizeof(char)*256);


	// put each 
	num_arg = 0;
	for(int i =0;i<len;i++){
		if( (str[i] == 0x0a) || (str[i] == 0x00) || (i == len-1)){
			if(flag ==1){
				arg[num_arg][arg_len[num_arg]] = '\0';
				num_arg++;
			}
			break;
		}
		else if( (str[i] != ' ') && (str[i] != '\t')){
			flag = 1;
			arg[num_arg][arg_len[num_arg]] = str[i];
			arg_len[num_arg]++;
		}
		else if(flag == 1){
			flag = 0;
			num_arg++;
			arg_len[num_arg] = 0;
		}
	}
	name = arg[0];
}

int Xml_Tree::create_children(){

	vector<vector<Xml_Bracket>> children_xml_list;
	vector<Xml_Bracket> tmp_xml_list;	

	int nest = 0;
	int num_children = 0;
	for(int i =1;i<xml_list.size()-1;i++){
		if(xml_list[i].flag == 1){
			nest ++ ;
		}else{
			nest--;
		}
		tmp_xml_list.push_back(xml_list[i]);
		if(nest == 0){
			num_children++;
			children_xml_list.push_back(tmp_xml_list);
			tmp_xml_list.resize(0);
		}
	}

	//printf("num_childen is %d\n", num_children);

	for(int i =0;i<children_xml_list.size();i++){
		Xml_Tree tmp (children_xml_list[i], gen);
		children.push_back(tmp);
	}

	// if everything is done, there only remain two elements in parent branch
	tmp_xml_list = xml_list;
	xml_list.resize(0);
	xml_list.push_back(tmp_xml_list[0]);
	xml_list.push_back(tmp_xml_list[tmp_xml_list.size()-1]);

	return 0;
}

void Xml_Tree::lex(){
	
	int start,end;

	char *name;

	Xml_Bracket xml_bracket;

	for(int i=0;i<size;i++){
		if( ((start = bracket_start(i)) != -1) &&  ((end = bracket_end(start)) != -1)){

			// retrieve the string inside of the bracket
			name = (char*)malloc(sizeof(char) * (end-start));

			if(buf[start+1] == '/'){
				xml_bracket.flag = 1;
				for(int j =start+2;j<end;j++)
					name[j-start-2] = buf[j];
				xml_bracket.str_spacer(name, end-start-2);
			} else{
				xml_bracket.flag = 0;
				for(int j =start+1;j<end;j++)
					name[j-start-1] = buf[j];
				xml_bracket.str_spacer(name, end-start-1);
			}

			// analyze the string

		}else{
			return;
		}
		i = end;

		xml_list.push_back(xml_bracket);
	}
}

// warning: this load program is not compatible with more tahan 256 bytes.
void Xml_Tree::load(const char filename[]){

	fd = open( filename, O_RDONLY);

	if(fd == -1){
		write(1, "no such file\n", 13);
		exit(-1);
	}

	size = read(fd, buf, BUF_SIZE);


	
	//for(int i = 0; i< BUF_SIZE;i++);
}

void Xml_Tree::show(){
	int nest = 0;
	printf("xml list size is %d\n", xml_list.size());
	for(int i =0;i<xml_list.size();i++){

		// show the name of bracket
		if(xml_list[i].flag == 0){
			for(int j=0;j<nest;j++)
				write(1,"  ",2);
			nest++;
		}else{
			nest--;
			for(int j=0;j<nest;j++)
				write(1,"  ",2);
		}
		printf("%s\n", xml_list[i].name);

		// show the information of bracket
		for(int j = 1; j < xml_list[i].num_arg; j++){
			for(int j=0;j<nest;j++)
				write(1,"  ",2);
			printf("- %s\n",  xml_list[i].arg[j]);
		}
	}
}

void Xml_Tree::show_buf(){
	write(1, buf, size);
}

char *Xml_Tree::get_data(char* target, char *keyword){

	char* str = (char*)malloc(64);

				//printf("len is %d\n", children[i].xml_list[0].arg_len[1]);
	for(int i =0;i<children.size();i++){
			if( str_match(children[i].xml_list[0].name, target, strlen(target)) == 1){
				int off;
				for(int k=0;k<children[i].xml_list[0].arg_len[1]; k++){
					if( children[i].xml_list[0].arg[1][k] == '\"'){
						off = k+1; break;
					}
				}

				//printf("off is %d\n", off);
				for(int k=0;k<children[i].xml_list[0].arg_len[1] -off; k++){
					if( children[i].xml_list[0].arg[1][k+off] == '\"') {
						str[k]='\0';
						break;
					}
					str[k] = children[i].xml_list[0].arg[1][k+off];
				}
			}
		}
	return str;
}

char *Xml_Tree::get_data_by_key(char* name, char *keyword, char *value, char *target){
	char* str = (char*)malloc(64);

	int off;

	int pt = -1;

			//printf("len is %d\n", children[i].xml_list[0].arg_len[1]);
	for(int i =0;i<children.size();i++){
		if( str_match(children[i].xml_list[0].name, name, strlen(name)) == 1){

			for(int j =1;j<children[i].xml_list[0].num_arg;j++){
				if( str_match(children[i].xml_list[0].arg[j], keyword, strlen(keyword)) == 1){

				// take the value of keyword
				for(int k = 0; k < children[i].xml_list[0].arg_len[j]; k++){
					if( children[i].xml_list[0].arg[j][k] == '\"'){
						off = k+1; break;
					}
				}

				//printf("off is %d\n", off);
				for(int k=0;k<children[i].xml_list[0].arg_len[j] -off; k++){
					if( children[i].xml_list[0].arg[j][k+off] == '\"') {
						str[k]='\0';
						break;
					}
					str[k] = children[i].xml_list[0].arg[1][k+off];
				}

				}
				if( str_match( value, str, strlen(str)) == 1)
					pt = i;
			}
		}
	}
	
	if( pt ==-1){
		return (char*)"-1";
	}

	// now we found the child that matches name and id.
	for(int j =1;j<children[pt].xml_list[0].num_arg;j++){
		if( str_match(children[pt].xml_list[0].arg[j], target, strlen(target)) == 1){

 			for(int k = 0; k < children[pt].xml_list[0].arg_len[j]; k++){
				if( children[pt].xml_list[0].arg[j][k] == '\"'){
					off = k+1;
					break;
				}
			}
			for(int k = 0; k < children[pt].xml_list[0].arg_len[j] - off; k++){
				if( children[pt].xml_list[0].arg[j][k+off] == '\"') {
					str[k]='\0';
					break;
				}
				str[k] = children[pt].xml_list[0].arg[j][k+off];
			}
			return str;
		}
	}
	return (char*)"-1";
}

// input format is value, decimal
char *Xml_Tree::get_data_by_id(char* name, int value, char *target){
	char* buf;
	int v = value;
	int digits =1;

	while(1){
		if((int)v /10 == 0)
			break;
		v = v / 10;
		digits++;
	}
	buf = (char*)malloc(digits+2);

	for(int j=0;j<digits;j++)
		buf[digits-j-1] = 0x30 + ((value/(int)pow(10,j))%10);
	buf[digits] = 0x00;

	return get_data_by_key( name, (char*)"id=", buf, target);
}





