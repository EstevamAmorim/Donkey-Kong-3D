//Pressione 'e' para subir nas escadas
//Espaco para pular
//Setas movimentam o personagem

//Modelos e texturas utilizados são do jogo Super Smash Bros de Nintendo Wii

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <modelos.h>

#define T_MILI 5 //Tempo que o jogo eh atualizado
#define DIST 10 //Distancia da camera para o personagem

unsigned int id_texturas[QUANT_TEX];
unsigned char use_gouraud = 1;
unsigned char use_depth_test = 1;
unsigned char use_light = 1;
unsigned char use_material = 1;

bool left = false, right = false, up= false, down = false, space = false;
float aceleration = 0;
bool on_ladder = false;
bool win = false,  lose = false;
int nivel = 0;
int cont = 0;

bool avalia(int l, int i, int j){
    if(l >= 0 && l < CEN_LENGHT && i >= 0 && i < CEN_HEIGHT && j >= 0 && j < CEN_WIDTH) return true;

    return false;
}

void troca(int l1, int i1, int j1, int l2, int i2, int j2) {
    int aux;
    float aux1;

    if(avalia(l1,i1,j1) && avalia(l2,i2,j2) && cenario[l2][i2][j2].modelo == -1) {
        aux = cenario[l1][i1][j1].modelo;
        cenario[l1][i1][j1].modelo = cenario[l2][i2][j2].modelo;
        cenario[l2][i2][j2].modelo = aux;

        aux1 = cenario[l1][i1][j1].rotacao;
        cenario[l1][i1][j1].rotacao = cenario[l2][i2][j2].rotacao;
        cenario[l2][i2][j2].rotacao = aux1;

        aux1 = cenario[l1][i1][j1].translacao[0];
        cenario[l1][i1][j1].translacao[0] = cenario[l2][i2][j2].translacao[0];
        cenario[l2][i2][j2].translacao[0] = aux1;

        aux1 = cenario[l1][i1][j1].translacao[1];
        cenario[l1][i1][j1].translacao[1] = cenario[l2][i2][j2].translacao[1];
        cenario[l2][i2][j2].translacao[1] = aux1;

        aux1 = cenario[l1][i1][j1].translacao[2];
        cenario[l1][i1][j1].translacao[2] = cenario[l2][i2][j2].translacao[2];
        cenario[l2][i2][j2].translacao[2] = aux1;

    }
}

void delete_cen(int l, int i, int j) {
    if(avalia(l,i,j)) {
        cenario[l][i][j].modelo = -1;
        cenario[l][i][j].rotacao = 0.0;
        cenario[l][i][j].translacao[0] = 0.0;
        cenario[l][i][j].translacao[1] = 0.0;
        cenario[l][i][j].translacao[2] = 0.0;
    }
}

bool busca(int l, int i, int j) {
    if(l > 0 && i > 0 && j > 0) {
        if(cenario[l][i-1][j].modelo != -1.0
           || cenario[l][i-1][j-1].modelo != -1.0
           || cenario[l][i-1][j+1].modelo != -1.0
           || cenario[l-1][i-1][j].modelo != -1.0
           || cenario[l+1][i-1][j].modelo != -1.0
           || cenario[l-1][i-1][j-1].modelo != -1.0
           || cenario[l+1][i-1][j-1].modelo != -1.0
           || cenario[l-1][i-1][j+1].modelo != -1.0
           || cenario[l+1][i-1][j+1].modelo != -1.0
           ) return true;
    }
    else if(l > 0 && i > 0) {
        if(cenario[l][i-1][j].modelo != -1.0
           || cenario[l][i-1][j+1].modelo != -1.0
           || cenario[l-1][i-1][j].modelo != -1.0
           || cenario[l+1][i-1][j].modelo != -1.0
           || cenario[l-1][i-1][j+1].modelo != -1.0
           || cenario[l+1][i-1][j+1].modelo != -1.0
           ) return true;
    }else if(j > 0 && i > 0) {
        if(cenario[l][i-1][j].modelo != -1.0
           || cenario[l][i-1][j-1].modelo != -1.0
           || cenario[l][i-1][j+1].modelo != -1.0
           || cenario[l+1][i-1][j].modelo != -1.0
           || cenario[l+1][i-1][j-1].modelo != -1.0
           || cenario[l+1][i-1][j+1].modelo != -1.0
           ) return true;
    }

    return false;
}

void camera_rotate(float angle , float distancia, float x_center, float z_center) {
    float ang = angle*M_PI/180.0;

    coords_camera[0] = distancia*cos(ang) + x_center;
    coords_camera[2] = distancia*sin(ang) + z_center;
}

void ajust_camera() {
    if(coords_char[1] <= 10 && nivel != 1) {
        coords_camera[1] = 10;
        nivel = 1;
        camera_rotate(180,DIST,0,4);
    } else if(coords_char[1] > 10 && coords_char[1] <= 20 && nivel != 2) {
        coords_camera[1] = 20;
        nivel = 2;
        camera_rotate(270,DIST,36,0);
    } else if(coords_char[1] > 20 && coords_char[1] <= 30 && nivel != 3) {
        coords_camera[1] = 30;
        nivel = 3;
        camera_rotate(0.0,DIST,39,34);
    } else if(coords_char[1] > 30 && coords_char[1] <= 40 && nivel != 4) {
        coords_camera[1] = 40;
        nivel = 4;
        camera_rotate(90.0,DIST,6,36);
    }
}

void gera_bar() {
    float r = rand()%3;
    if(cont == 100) {
        if(r == 0 && cenario[10][31][4].modelo == -1) cenario[10][31][4].modelo = 3;
        else if(r == 1 && cenario[10][31][6].modelo == -1) cenario[10][31][6].modelo = 3;
        else if(cenario[10][31][8].modelo == -1) cenario[10][31][8].modelo = 3;
    }
}

//Funcao incompleta, seria ela quem moveria os barris na tela e caso colidissem com o personagem, ele sofreria dano
void move_bar() {
    int l,i,j;
    int aux_l, aux_i, aux_j;
    float r;

    for(l=0;l<CEN_LENGHT;l++) {
		for(i=0;i<CEN_HEIGHT;i++) {
			for(j=0;j<CEN_WIDTH;j++) {
                 if(cenario[l][i][j].modelo == 3) {
                    r = rand()%4;
                    if(i==1 && (j>3 && j<34)) {
                        cenario[l][i][j].translacao[0] -= 0.2;
                    }

                    else if(i==11 && l > 1 && l<31 && j > 33 && j < 39)  {
                        if(((l == 6 && r > 1) || l == 2) && cenario[l][i][j].translacao[2] < 0.2) {
                                if(l == 6 && r == 2) cenario[l][i][j].translacao[2] -= 2;

                                else if(l == 2 && r == 1) cenario[l][i][j].translacao[2] += 2;

                                cenario[l][i][j].translacao[0]-=(j-33);
                                cenario[l][i][j].rotacao = 90.0;
                        }

                        else {
                            cenario[l][i][j].translacao[2] -= 0.2;
                        }
                    }

                    else if( i==21 && j>8 && j<39 && l>31 && l<37)  {
                        if( ((j == 34 && r > 1) || j == 38) && cenario[l][i][j].translacao[0] < 0.2) {
                                if(j == 34 && r == 2) cenario[l][i][j].translacao[0] += 2;

                                else if(j == 38 && r == 1) cenario[l][i][j].translacao[0] -= 2;

                                cenario[l][i][j].translacao[2]-=(l-30);
                                cenario[l][i][j].rotacao = 0.0;
                        }

                        cenario[l][i][j].translacao[0] += 0.2;
                    }

                    else if( i==31 && l>7 && l<38 && j>3 && j<9 )  {
                        if( ((l == 32 && r > 1) || l == 36) && cenario[l][i][j].translacao[2] < 0.2) {
                                if(l == 32 && r == 2) cenario[l][i][j].translacao[2] += 2;

                                else if(l == 36 && r == 1) cenario[l][i][j].translacao[2] -= 2;

                                cenario[l][i][j].translacao[0]+=(10-j);
                                cenario[l][i][j].rotacao = 90.0;
                        }

                        cenario[l][i][j].translacao[2] += 0.2;
                    }

                    else {
                        cenario[l][i][j].translacao[1] -= 0.2;
                    }


                    aux_j = round(j + cenario[l][i][j].translacao[0]);
                    aux_i = round(i + cenario[l][i][j].translacao[1]);
                    aux_l = round(l + cenario[l][i][j].translacao[2]);

                    cenario[l][i][j].translacao[0] = j + cenario[l][i][j].translacao[0] - aux_j;
                    cenario[l][i][j].translacao[1] = i + cenario[l][i][j].translacao[1]  - aux_i;
                    cenario[l][i][j].translacao[2] = l + cenario[l][i][j].translacao[2] - aux_l;

                    if(aux_l > 1 && l < 7 && aux_i == 1 && aux_j == 3) delete_cen(l,i,j);
                    else if(cenario[aux_l][aux_i][aux_j].modelo == 0) lose = true;
                    else {troca(l,i,j,aux_l,aux_i, aux_j);}
                 }
			}
		}
    }
}

void up_ladder() {
    float j = coords_char[0], i = coords_char[1], l = coords_char[2];
    float aux_j,aux_i,aux_l,aux;

    if(!on_ladder) {
        for(aux_i = i-11; aux_i <= i+10; aux_i++) {
            for(aux_l = l-2; aux_l <= l + 2; aux_l++) {
                for(aux_j = j-2; aux_j <= j+2; aux_j++) {
                    if(avalia(aux_l,aux_i,aux_j) && cenario[(int)aux_l][(int)aux_i][(int)aux_j].modelo == 2 && (!on_ladder)) {
                            cenario[(int)l][(int)i][(int)j].translacao[0] = 0.0;
                            cenario[(int)l][(int)i][(int)j].translacao[1] = 0.0;
                            cenario[(int)l][(int)i][(int)j].translacao[2] = 0.0;

                            cenario[(int)l][(int)i][(int)j].rotacao = cenario[(int)aux_l][(int)aux_i][(int)aux_j].rotacao - 180.0;
                            if(cenario[(int)l][(int)i][(int)j].rotacao < 0.0) cenario[(int)l][(int)i][(int)j].rotacao += 360.0;

                            coords_char[0] = aux_j;
                            coords_char[2] = aux_l;
                            if(aux_i == i-11) coords_char[1] = i-2;
                            coords_char[3] = cenario[(int)l][(int)i][(int)j].rotacao;
                            coords_char[4] = 0.0;
                            coords_char[5] = 0.0;
                            coords_char[6] = 0.0;


                            troca(l,i,j,aux_l,coords_char[1],aux_j);

                            on_ladder = true;
                    }
                }
            }
        }
    }
}

//Com base na normal do vetor entre a camera e o personagem a direcao em que ele se movimenta com as setas eh definida
//esquerda, direita, frente e tras sao em relacao a camera, a quem esta observando
void move_char() {
    if(left || up || right || down) {
        float j = coords_char[0];
        float i = coords_char[1];
        float l = coords_char[2];
        float x_dir = coords_camera[0] - j - coords_char[4];
        float z_dir = coords_camera[2] - l - coords_char[6];
        float dist;
        float aux_x = x_dir;
        float aux_z = z_dir;
        int model;

        if(left && up) {x_dir = aux_z - aux_x; z_dir = -aux_x + (-aux_z);}
        else if(up && right) {x_dir = -aux_x + (-aux_z); z_dir = -aux_z + aux_x;}
        else if(right && down) {x_dir = -aux_z + x_dir; z_dir = aux_x + z_dir;}
        else if(down && left) {x_dir = x_dir + aux_z; z_dir = z_dir + -aux_x;}
        else if(left) {x_dir = aux_z; z_dir = -aux_x;}
        else if(right) {x_dir = -aux_z; z_dir = aux_x;}
        else if(up) {x_dir = -aux_x; z_dir = -aux_z;}

        dist = sqrt(pow(x_dir,2) + pow(z_dir,2));
        x_dir = (x_dir/dist)*0.1;
        z_dir = (z_dir/dist)*0.1;


        if(z_dir < (-0.05)) {
            if(x_dir <= -0.05) coords_char[3] = 225.0;
            else if( x_dir > -0.05 && x_dir <= 0.05) coords_char[3] = 180.0;
            else {coords_char[3] = 135.0;}

        }else if(z_dir >= 0.05) {
            if(x_dir < -0.05) coords_char[3] = 315.0;
            else if( x_dir >= -0.05 && x_dir < 0.05) coords_char[3] = 0.0;
            else {coords_char[3] = 45.0;}
        }else {
            if(x_dir <= -0.05) coords_char[3] = 270.0;
            else {coords_char[3] = 90.0;}
        }

        x_dir += j + coords_char[4];
        z_dir += l + coords_char[6];

        aux_x = round(x_dir);
        aux_z = round(z_dir);

        x_dir -= aux_x;
        z_dir -= aux_z;

        if(avalia(aux_z,i,aux_x)) {
            model = cenario[(int)aux_z][(int)i][(int)aux_x].modelo;
            if(model == -1 || model == 0) {
                coords_char[0] = aux_x;
                coords_char[2] = aux_z;
                coords_char[4] = x_dir;
                coords_char[6] = z_dir;
                cenario[(int)l][(int)i][(int)j].rotacao = coords_char[3];
                cenario[(int)l][(int)i][(int)j].translacao[0] = x_dir;
                cenario[(int)l][(int)i][(int)j].translacao[2] = z_dir;
                troca(l,i,j,aux_z,i,aux_x);
            } else if(model == 3){
                lose = true;
            }
        }
    }
}

//A gravidade aplicada ao personagem tem acerelacao de 0.01 blocos/milisegundo
//O mario salta com uma aceleracao de 0.3 blocos/milisegundo
void move_char_falling() {
    float j = coords_char[0], i = coords_char[1], l = coords_char[2];
    float i_new = 0;
    float deslocamento;

    if(!space && aceleration > 0) aceleration = 0.0;

    if(space && busca(l,i,j) && coords_char[5] == 0.0) aceleration = 0.3;

    deslocamento = coords_char[5] + aceleration;

    while(deslocamento < 0.0) {
        i_new--;
        if(busca(l,i+i_new+1,j)) {aceleration = 0.0; deslocamento = 0.0;i_new++;}
        else {deslocamento += 1.0;}
    }
     while(deslocamento > 1.0) {
        i_new++;
        if(busca(l,i+i_new+1,j)) {aceleration = 0.0; deslocamento = 0.0;i_new--;}
        else {deslocamento-=1.0;}
    }

    i_new += i;

    if(avalia(l,i_new,j)) {
        coords_char[1] = i_new;
        coords_char[5] = deslocamento;
        cenario[(int)l][(int)i][(int)j].translacao[1] = deslocamento;

        troca(l,i,j,l,i_new,j);
    } else {
        lose = true;
    }

    aceleration-=0.01;
}

void move_char_ladder() {
    float j = coords_char[0], i = coords_char[1], l = coords_char[2];
    float aux_j,aux_i,aux_l,r;

    bool aux = false;

    if(up && avalia(l,floorf(i + coords_char[5] + 0.2),j) && (cenario[(int)l][(int)floorf(i + coords_char[5] + 0.2)][(int)j].modelo == -1 ||
                                                              cenario[(int)l][(int)floorf(i + coords_char[5] + 0.2)][(int)j].modelo == 0)) {
        coords_char[5] += 0.2;
        if(coords_char[5] > 1.0) {
            coords_char[5]-=1.0;
            coords_char[1]++;
        }
        cenario[(int)l][(int)i][(int)j].translacao[1] = coords_char[5];
        troca(l,i,j,l,coords_char[1],j);
    }
    else if(down && avalia(l,floorf(i + coords_char[5] - 0.2),j) && (cenario[(int)l][(int)floorf(i + coords_char[5] - 0.2)][(int)j].modelo == -1 ||
                                                                    cenario[(int)l][(int)floorf(i + coords_char[5] - 0.2)][(int)j].modelo == 0)) {
        coords_char[5] -= 0.2;
        if(coords_char[5] < 0.0) {
            coords_char[5]+=1.0;
            coords_char[1]--;
        }
        cenario[(int)l][(int)i][(int)j].translacao[1] = coords_char[5];
        troca(l,i,j,l,coords_char[1],j);
    }

     for(aux_i = coords_char[1]-10; aux_i <= coords_char[1]+10; aux_i++) {
        if(avalia(l,aux_i,j) && cenario[(int)l][(int)aux_i][(int)j].modelo == 2) aux = true;
     }

     if(!aux) {
        if(up && avalia(l,coords_char[1]-11,j)) {
            r = cenario[(int)l][(int)(coords_char[1] - 11)][(int)j].rotacao;
            if(r == 270.0) coords_char[0]++;
            else if(r == 180.0) coords_char[2]++;
            else if(r == 90.0) coords_char[0]--;
            else if(r == 0.0) coords_char[2]--;

            troca(l,coords_char[1],j,coords_char[2],coords_char[1],coords_char[0]);

        }
        on_ladder = false;
     }
}

void reshape_callback(int w, int h){
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();

    //glOrtho (0, w, 0, h, 1 ,-1);
    gluPerspective(30, (float)w/(float)h, 1.0, 130.0);

    glMatrixMode (GL_MODELVIEW);
}

void timer_callback(int value){
    glutTimerFunc(value, timer_callback, value);
    srand(time(NULL));
    cont++;
    if(cont > 100) cont = 0;
    glutPostRedisplay();
}

void enable_textura(int text) {
    glBindTexture(GL_TEXTURE_2D, id_texturas[text]);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,texturas[text].width,
                 texturas[text].height,
                 0, GL_RGB, GL_UNSIGNED_BYTE, texturas[text].tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_CLAMP);

    glEnable(GL_TEXTURE_2D);
}

void draw(int lengh, int height, int width){
    int i,j;
    int model = cenario[lengh][height][width].modelo;
    glRotatef(cenario[lengh][height][width].rotacao,0.0,1.0,0.0);
    enable_textura(modelos[model].textura);
    glPushMatrix();
        glBegin(GL_TRIANGLES);
            for (i=0; i< modelos[model].num_faces; i++){
                for (j=0; j<3; j++){
                   glTexCoord2fv(modelos[model].tex_coords[modelos[model].faces[i][j][1]]);

                   glNormal3fv(modelos[model].vert_normals[modelos[model].faces[i][j][2]]);

                   glVertex3fv(modelos[model].vertices[modelos[model].faces[i][j][0]]);
                }
            }
         glEnd();
     glPopMatrix();
     glDisable(GL_TEXTURE_2D);

}

void update() {
    int l,i,j;

    for(l=0;l<CEN_LENGHT;l++) {
		for(i=0;i<CEN_HEIGHT;i++) {
			for(j=0;j<CEN_WIDTH;j++) {
                if( cenario[l][i][j].modelo != -1) {
                    if(cenario[l][i][j].modelo != 1) {
                        glPushMatrix();
                            glTranslated(j + cenario[l][i][j].translacao[0],i + cenario[l][i][j].translacao[1],l + cenario[l][i][j].translacao[2]);
                            draw(l,i,j);
                        glPopMatrix();
                    } else if (cenario[l][i][j].translacao[1] == -1.0) {
                        glPushMatrix();
                            glTranslated(j,i,l);
                            draw(l,i,j);
                        glPopMatrix();
                    }
                }
            }
        }
    }

}

void display_callback(void){
    float angulo;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if(!avalia(coords_char[2],coords_char[1],coords_char[0])) lose = true;

    if(coords_char[2] == 8 && coords_char[1] == 31 && coords_char[0] == 6) win = true;

    glScalef(-1, 1, 1);

    if(!lose && !win) {
        gera_bar();

        if(on_ladder) {
            move_char_ladder();
        } else {
            move_char();
            move_char_falling();
        }

        move_bar();

    }

    ajust_camera();
    gluLookAt(coords_camera[0], coords_camera[1], coords_camera[2] , coords_char[0] + coords_char[4], coords_char[1] + coords_char[5], coords_char[2] + coords_char[6], 0.0,1.0,0.0);

    update();

    glutSwapBuffers();
}

void keyboard_callback(unsigned char key, int x, int y){
    switch(key) {
        case 27:
            exit(0); /* Esc: sai do programa */
            break;
        case 32:
            space = true;
            on_ladder = false;
            break;
        case 'e':
            up_ladder();
            break;
        case 's':
            if(win || lose){ inicia(models_names,text_names); win = false; lose = false;}
            break;

    }
}

void keyboardUp_callback(unsigned char key, int x, int y) {
    switch(key){
        case 32:
            space = false;
            break;
    }
}

void keyboard_callback_special(int key, int x, int y){
    switch(key){
        case 1:
            if (use_light==1){
               use_light=0;
               glDisable(GL_LIGHTING);
            }else{
               use_light=1;
               glEnable(GL_LIGHTING);
            }
            break;

        case 2:
            if (use_gouraud==1){
               use_gouraud=0;
               glShadeModel(GL_FLAT);

            }else{
               use_gouraud=1;
               glShadeModel(GL_SMOOTH);
            }
            break;

        case 3:
            if (use_depth_test==1){
               use_depth_test=0;
               glDisable(GL_DEPTH_TEST);
            }else{
               use_depth_test=1;
               glEnable(GL_DEPTH_TEST);
            }
            break;
        case GLUT_KEY_LEFT:
            left = true;
            break;
        case GLUT_KEY_UP:
            up = true;
            break;
        case GLUT_KEY_RIGHT:
            right = true;
            break;
        case GLUT_KEY_DOWN:
            down = true;
            break;


    }
}

void keyboardUp_callback_special(int key, int x, int y) {
    switch(key){
        case GLUT_KEY_LEFT:
            left = false;
            break;
        case GLUT_KEY_UP:
            up = false;
            break;
        case GLUT_KEY_RIGHT:
            right = false;
            break;
        case GLUT_KEY_DOWN:
            down = false;
            break;
    }
}

void init_glut(const char *nome_janela, int argc, char** argv){
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {25.0};
    GLfloat light_position[] = {50.0, 50.0, 50.0, 1.0};

    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(100,100);
    glutCreateWindow(nome_janela);
    glutKeyboardFunc(keyboard_callback);
    glutKeyboardUpFunc(keyboardUp_callback);
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutSpecialFunc(keyboard_callback_special);
    glutSpecialUpFunc(keyboardUp_callback_special);
    glGenTextures(QUANT_TEX, id_texturas);
    glutTimerFunc(T_MILI, timer_callback,T_MILI);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);


    glShadeModel(GL_SMOOTH);

    glEnable(GL_DEPTH_TEST);


    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(1.0, 1.0, 1.0);

}

int main(int argc, char** argv){
    inicia(models_names,text_names);

    init_glut("Desenhando Objetos em 3D", argc, argv);

    glutMainLoop();

    return 0;
}
