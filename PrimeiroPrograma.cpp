#include <GL/glut.h>
#include <iostream>
#include <fstream>

// Identificador da textura e ângulo de rotação
GLuint texture;
float angle = 0.0;
float angleIncrement = 1.0;
bool Animando = true;

// Estruturas para cabeçalho BMP
#pragma pack(push, 1)
struct BMPHeader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offBits;
};

struct BMPInfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t sizeImage;
    int32_t xPelsPerMeter;
    int32_t yPelsPerMeter;
    uint32_t clrUsed;
    uint32_t clrImportant;
};
#pragma pack(pop)

// Função para carregar a textura BMP
bool loadBMP(const char* filename) {
    BMPHeader header;
    BMPInfoHeader infoHeader;
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }

    file.read((char*)&header, sizeof(header));
    file.read((char*)&infoHeader, sizeof(infoHeader));

    if (header.type != 0x4D42) {
        std::cerr << "Not a valid BMP file: " << filename << std::endl;
        return false;
    }

    unsigned char* data = new unsigned char[infoHeader.sizeImage];
    file.seekg(header.offBits, std::ios::beg);
    file.read((char*)data, infoHeader.sizeImage);
    file.close();

    // Converter de BGR para RGB
    for (unsigned int i = 0; i < infoHeader.sizeImage; i += 3) {
        std::swap(data[i], data[i + 2]);
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, infoHeader.width, infoHeader.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    delete[] data;
    return true;
}

// Função para configurar a iluminação
void Ilumina() {
    // Ativa o modelo de iluminação global
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Configura a posição da luz
    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Configura a cor da luz
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Configura as propriedades do material do cubo
    GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// Função para exibir créditos
void Creditos() {
    std::cout << "Trabalho Terceira Unidade Computacao Grafica\n";
    std::cout << "Autores: Pedro Eduardo Morais Ribeiro da Silva\n";
    std::cout << "Prof. Dr. Francisco Chagas de Lima Junior\n";
}

// Função de inicialização
void Inicializa() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    Ilumina();  // Configura a iluminação
    if (!loadBMP("texture.bmp")) {
        std::cerr << "Failed to load texture" << std::endl;
    }
}

// Função de exibição
void Desenhar() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -5.0);
    glRotatef(angle, 1.0, 1.0, 1.0);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);

    // Face dianteira
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, 1.0);

    // Face traseira
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, -1.0);

    // Face superior
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, -1.0);

    // Face inferior
    glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, 1.0);

    // Face direita
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 1.0, -1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 1.0);

    // Face esquerda
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, -1.0, -1.0);
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0, -1.0, 1.0);
    glTexCoord2f(1.0, 1.0); glVertex3f(-1.0, 1.0, 1.0);
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 1.0, -1.0);

    glEnd();
    glutSwapBuffers();
}

// Função temporizadora
void temporizador(int value) {
    if (Animando) {
        angle += angleIncrement;
        if (angle > 360.0) {
            angle -= 360.0;
        }
        else if (angle < -360.0) {
            angle += 360.0;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, temporizador, 0);
}

// Função de redimensionamento
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

// Função de teclado
void teclado(unsigned char tecla, int x, int y) {
    switch (tecla) {
    case '1':
        angleIncrement = -1.0;  // Rotação para a esquerda
        break;
    case '2':
        angleIncrement = 1.0;   // Rotação para a direita
        break;
    case 27: // Tecla ESC
        exit(0);
        break;
    }
}

// Função de tratamento do menu
void menu(int option) {
    switch (option) {
    case 1:
        Creditos();
        break;
    case 2:
        Animando = !Animando;  // Inicia/para a animação
        break;
    }
}

// Função de inicialização do menu
void criarMenu() {
    glutCreateMenu(menu);
    glutAddMenuEntry("Créditos", 1);
    glutAddMenuEntry("Iniciar/Parar Animação", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Trabalho Terceira Unidade Computação Gráfica");
    Inicializa();
    criarMenu();  // Cria o menu
    glutDisplayFunc(Desenhar);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutTimerFunc(0, temporizador, 0);
    glutMainLoop();
    return 0;
}
