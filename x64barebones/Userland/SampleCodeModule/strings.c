int strlen(char * s){
	int c = 0;
	while(*(s++) != 0){
		c++;
	}

	return c;
}


int strcmp(char * s1, char * s2){
	while(*s1 && *s2){
		s1++;
		s2++;
	}
	return *s1 - *s2;
}


int starts_with(char * str, char * start){
	while(*str != 0 && *start != 0){
		if(*(str++) != *(start++)){
			return 0;
		}
	}
	return *start == 0;
}
