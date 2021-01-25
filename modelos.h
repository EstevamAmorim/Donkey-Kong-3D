#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#define QUANT_MODELOS 5
#define QUANT_TEX 5
#define CEN_WIDTH 40
#define CEN_HEIGHT 60
#define CEN_LENGHT 40
#define CHAR_ID 0

struct modelo{
    char *name;
    int num_vertices;
    int num_faces;
    int num_tex_coord;
    int num_vert_normal;
    float ** vertices;
    int *** faces;
    float ** tex_coords;
    float ** vert_normals;
    float ** face_normals;
    int textura;
};

struct text{
    GLsizei height;
    GLsizei width;
    unsigned char * tex;
};

struct elemento{
    float rotacao;
    float translacao[3];
    int modelo;
};

elemento cenario[CEN_LENGHT][CEN_HEIGHT][CEN_WIDTH]; /*O cenario eh uma matriz de tres dimensoes, cada elemento
                                                       corresponde a um bloco exibido na tela e contem um indice do vetor
                                                       de modelos e a translacao desse modelo em relacao ao bloco*/
modelo modelos[QUANT_MODELOS];
text texturas[QUANT_TEX];
GLfloat coords_char[7];
GLfloat coords_camera[7];

char * models_names[QUANT_MODELOS][2] = {{"Modelos/Mario Smash Bross/Jogo/mario1.obj", (char *)0}  ,   {"Modelos/Objetos/Viga/v1.obj",(char *)1}, {"Modelos/Objetos/Escada/escada.obj",(char *)2}, {"Modelos/Objetos/Barril/bar.obj",(char *)3},{"Modelos/Donkey Kong Smash Bross/Jogo/Donkey.obj",(char *)4} };
char * text_names[QUANT_TEX] = {"Modelos/Mario Smash Bross/Jogo/s.pnm" , "Modelos/Objetos/Viga/viga.pnm",  "Modelos/Objetos/Escada/escada.pnm", "Modelos/Objetos/Barril/bar.pnm", "Modelos/Donkey Kong Smash Bross/Jogo/d.pnm"};

float** alocarMatriz2D(int linhas,int colunas){
    int i;

    float **m = (float**)malloc(linhas * sizeof(float*));
    for (i = 0; i < linhas; i++){
       m[i] = (float*) malloc(colunas * sizeof(float));
    }
    return m;
}

int*** alocarMatriz3D(int linhas,int colunas,int comprimento){
  int i,j;

  int ***m = (int***)malloc(linhas * sizeof(int**));
  for (i = 0; i < linhas; i++){
       m[i] = (int**) malloc(colunas * sizeof(int*));
   	   for (j = 0; j < colunas; j++){
   	   		m[i][j] = (int*) malloc(comprimento * sizeof(int));
       }
  }
	return m;
}

void load(char *path, int text,modelo *m) {
    int num_vert = 0;
    int num_face = 0;
    int num_tex_coord = 0;
    int num_vert_normal = 0;
    int c1,c2;
    char buff[BUFSIZ];

    FILE *p;
    p = fopen(path, "r");

    if (p == NULL) {
        printf("ERRO: Modelo não encontrado.\n");
        exit(0);
    }

    while(!feof(p)){
        c1 = getc(p);
        c2 = getc(p);
        if(c1 == 'v' && c2 == ' ')
            num_vert++;
        else if(c1 == 'v' && c2 == 't')
            num_tex_coord++;
        else if(c1 == 'v' && c2 == 'n')
            num_vert_normal++;
        else if(c1 == 'f' && c2 == ' ')
            num_face++;

        fgets(buff,BUFSIZ,p);
    }

    m->name = path;
    m->num_vertices = num_vert;
    m->num_faces = num_face;
    m->num_tex_coord = num_tex_coord;
    m->num_vert_normal = num_vert_normal;
    m->vertices = alocarMatriz2D(num_vert,3);
    m->faces = alocarMatriz3D(num_face,3,3);
    m->tex_coords = alocarMatriz2D(num_tex_coord,2);
    m->vert_normals = alocarMatriz2D(num_vert_normal,3);
    m->textura = text;

    rewind(p);
    num_vert = 0;
    num_face = 0;
    num_tex_coord = 0;
    num_vert_normal = 0;

    while(!feof(p)){
        c1 = getc(p);
        c2 = getc(p);
        if(c1 == 'v' && c2 == ' ') {
            fscanf(p,"%f %f %f",&(m->vertices[num_vert][0]),
                                &(m->vertices[num_vert][1]),
                                &(m->vertices[num_vert][2]));
            num_vert++;
        }
        else if(c1 == 'v' && c2 == 't') {
            fscanf(p," %f %f",&(m->tex_coords[num_tex_coord][0]),
                                &(m->tex_coords[num_tex_coord][1]));
            num_tex_coord++;
        }
        else if(c1 == 'v' && c2 == 'n') {
            fscanf(p," %f %f %f",&(m->vert_normals[num_vert_normal][0]),
                                &(m->vert_normals[num_vert_normal][1]),
                                &(m->vert_normals[num_vert_normal][2]));

            num_vert_normal++;
        }
        else if(c1 == 'f' && c2 == ' ') {
            fscanf(p,"%d/%d/%d %d/%d/%d %d/%d/%d",&m->faces[num_face][0][0],
                                                  &m->faces[num_face][0][1],
                                                  &m->faces[num_face][0][2],

                                                  &m->faces[num_face][1][0],
                                                  &m->faces[num_face][1][1],
                                                  &m->faces[num_face][1][2],

                                                  &m->faces[num_face][2][0],
                                                  &m->faces[num_face][2][1],
                                                  &m->faces[num_face][2][2]);
            m->faces[num_face][0][0]--;
            m->faces[num_face][0][1]--;
            m->faces[num_face][0][2]--;
            m->faces[num_face][1][0]--;
            m->faces[num_face][1][1]--;
            m->faces[num_face][1][2]--;
            m->faces[num_face][2][0]--;
            m->faces[num_face][2][1]--;
            m->faces[num_face][2][2]--;
            num_face++;
        }

        fgets(buff,BUFSIZ,p);
    }

    fclose(p);

}

void load_text(char *path, text * t) {
    int cont = 0;
    char buff[BUFSIZ];

    FILE *p;
    p = fopen(path, "r");

    if (p == NULL) {
        printf("ERRO: Textura não encontrada.\n");
        exit(0);
    }

    fgets(buff,BUFSIZ,p);

    fscanf(p,"%d %d",&(t->width),&(t->height));

    fgets(buff,BUFSIZ,p);
    fgets(buff,BUFSIZ,p);

    t->tex = (unsigned char*)malloc(t->height*t->width*3 * sizeof(unsigned char));

    while(!feof(p)){
        fscanf(p,"%d",&(t->tex[cont]));
        fgets(buff,BUFSIZ,p);
        cont++;
    }

    fclose(p);
}

void load_cenario() {
    int l,i,j;

    FILE *p;
    p = fopen("cenario.txt", "r");

    if (p == NULL) {
        printf("ERRO: Textura não encontrada.\n");
        exit(0);
    }

    for(l=0;l<CEN_LENGHT;l++) {
		for(i=0;i<CEN_HEIGHT;i++) {
			for(j=0;j<CEN_WIDTH;j++) {
				if(j==CEN_WIDTH-1) fscanf(p,"%d %f %f %f %f\n",&cenario[l][i][j].modelo,
                              &(cenario[l][i][j].rotacao),
                              &(cenario[l][i][j].translacao[0]),
                              &(cenario[l][i][j].translacao[1]),
                              &(cenario[l][i][j].translacao[2]));
				else{fscanf(p,"%d %f %f %f %f ",&cenario[l][i][j].modelo,
                                                &(cenario[l][i][j].rotacao),
                                                &(cenario[l][i][j].translacao[0]),
                                                &(cenario[l][i][j].translacao[1]),
                                                &(cenario[l][i][j].translacao[2]));}
                if(cenario[l][i][j].modelo == CHAR_ID) {
                        coords_char[0] = j;
                        coords_char[1] = i;
                        coords_char[2] = l;

                        coords_char[3] = cenario[l][i][j].rotacao;
                        coords_char[4] = cenario[l][i][j].translacao[0];
                        coords_char[5] = cenario[l][i][j].translacao[1];
                        coords_char[6] = cenario[l][i][j].translacao[2];

                }
			}
		}
		fscanf(p,"\n");
	}

	fclose(p);
}

void inicia(char *models_names[][2],char *text_names[]) {
    int i;

    for(i=0;i<QUANT_MODELOS;i++){
        load(models_names[i][0],(int) models_names[i][1],&modelos[i]);
    }
    for(i=0;i<QUANT_TEX;i++){
        load_text(text_names[i],&texturas[i]);
    }

    load_cenario();

};
