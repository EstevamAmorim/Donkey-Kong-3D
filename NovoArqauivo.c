#include <stdio.h>
#include <stdlib.h>



int main() {
	
	int i, j, l;
	
	
	FILE *p;
    p = fopen("cenario.txt", "w");

    if (p == NULL) {
        printf("ERRO: Modelo não encontrado.\n");
        exit(0); 
    }
    
    
    for(l=0;l<40;l++) {
		for(i=0;i<60;i++) {
			for(j=0;j<40;j++) {
				if(l== 4 && j==4 && i==1) {fprintf(p,"0 90.0 0.0 0.0 0.0 ");} 
                else if(i==0 && (j>3 && j<33) && (l<7 && l>1) && (l%2 == 0 && j%2 ==0)) {
					if(j == 18) {fprintf(p,"1 0.0 0.0 -1.0 0.0 ");}
					else {fprintf(p,"1 0.0 0.0 0.0 0.0 ");}
				
				}
				
				else if(i== 0 && j == 33 && (l == 2 || l == 6)) {fprintf(p,"2 270.0 0.0 0.0 0.0 ");}
				
				else if(i== 10 && l == 31 && (j == 34 || j == 38 )) {fprintf(p,"2 180.0 0.0 0.0 0.0 ");}
					
				else if(  (i==10 && j>33 && j < 39) && (l>1 && l<31) && l%2 == 0 && j%2 == 0 )  {
					if(l == 16) {fprintf(p,"1 90.0 0.0 -1.0 0.0");}
					else {fprintf(p,"1 90.0 0.0 0.0 0.0 ");}
				}
			
				else if( i==20 && (j>9 && j<39) && (l<37 && l>31) && (l%2 == 0 && j%2 ==0))  { 
					if(j == 24) {fprintf(p,"1 180.0 0.0 -1.0 0.0 ");}
					else {fprintf(p,"1 180.0 0.0 0.0 0.0 ");}
				}
				
				else if(j==39) {fprintf(p,"-1 0.0 0.0 0.0 0.0\n");}
				else{fprintf(p,"-1 0.0 0.0 0.0 0.0 ");}
			}
		}
		
		fprintf(p,"\n");
		
	}
	
	
	
	
	fclose(p);
	
	
	return 0;
}