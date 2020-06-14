#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <fstream>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QStack>

using namespace std;

static int edo;
static QStack<int> pilaEjecucion;

int cT = 0, cE = 0;
QList<QString> tokenLeido;
QList<QString> descripcionToken;
QList<QString> errorLeido;
QList<QString> descripcionError;


int M[19][32] = {
    /* 0 */     {   1,   2,   3, 500, 124, 500, 500, 108, 109,   9,  11,  12, 110, 111, 112,  17,  18,  13,  16,  15,  14, 125, 123, 126, 127, 128, 129, 500,   0,   0,   0, 130 },
    /* 1 */     {   1,   2,   2,   2, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
    /* 2 */     {   2,   2,   2,   2, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101 },
    /* 3 */     { 102, 102,   3, 102,   4, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102 },
    /* 4 */     { 501, 501,   5, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501 },
    /* 5 */     { 103, 103,   5, 103, 103,   6,   6, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103 },
    /* 6 */     { 502, 502,   8, 502, 502, 502, 502,   7,   7, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502 },
    /* 7 */     { 503, 503,   8, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503 },
    /* 8 */     { 104, 104,   8, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104 },
    /* 9 */     {  10,  10,  10,  10,  10,  10,  10,  10,  10, 504,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10,  10 },
    /* 10 */     { 505, 505, 505, 505, 105, 505, 505, 505, 505, 105, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505, 505 },
    /* 11 */     {  11, 11,   11,  11,  11,  11,  11,  11,  11,  11, 106,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11,  11 },
    /* 12 */     {  12, 12,   12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12,  12, 107,  12,  12 },
    /* 13 */     { 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 114, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113 },
    /* 14 */     { 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 116, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115, 115 },
    /* 15 */     { 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 118, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117, 117 },
    /* 16 */     { 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 120, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119, 119 },
    /* 17 */     { 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 121, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506, 506 },
    /* 18 */     { 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 122, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507 },
};

static int matrizPredictiva[45][46] = {
    {1,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508,508},
    {509,2,5,5,5,5,5,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,509,5,509,509},
    {510,3,4,4,4,4,4,510,510,4,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,510,4,510,510},
    {511,511,6,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511,511},
    {512,7,8,8,8,8,8,512,512,8,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,512,8,512,512},
    {513,513,9,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513,513},
    {514,514,514,514,514,514,514,10,11,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,514,11,514,514},
    {515,515,12,12,12,12,12,515,515,88,88,88,88,88,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515,515},
    {516,516,15,16,17,18,19,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516,516},
    {517,517,13,13,13,13,13,517,517,14,14,14,14,14,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,517,14,517,517},
    {518,518,518,518,518,518,518,518,518,518,518,518,518,518,20,21,22,23,24,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518,518},
    {519,519,25,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519,519},
    {520,520,520,520,520,520,520,27,27,520,520,520,520,520,520,520,520,520,520,26,520,520,520,520,520,520,520,520,520,520,520,520,520,520,520,520,520,520,520,520,520,520,520,27,520,520},
    {521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,28,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521,521},
    {522,522,522,522,522,522,522,30,30,522,522,522,522,522,522,522,522,522,522,29,522,522,522,522,522,522,522,522,522,522,522,522,522,522,522,522,522,522,522,522,522,522,522,30,522,522},
    {523,523,31,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523,523},
    {524,524,524,524,524,524,524,33,33,524,524,524,524,524,524,524,524,524,524,32,33,33,33,33,33,33,33,33,33,33,33,33,33,33,33,524,524,524,524,524,524,524,33,33,524,33},
    {525,525,34,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525,525},
    {526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,35,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526,526},
    {527,527,527,527,527,527,527,36,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,527,37,527,527,527,527,527,527,527,527,527,527,527},
    {528,528,528,528,38,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528,528},
    {529,529,529,529,529,39,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529,529},
    {530,530,40,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530,530},
    {531,531,531,531,531,531,531,42,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,531,41,531,531,531},
    {532,532,532,532,532,532,43,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532,532},
    {533,533,44,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,533,44,44,44,44,44,44,533,533,44,533},
    {534,534,534,534,534,534,534,46,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,534,45,534,534,534},
    {535,535,47,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,535,47,47,47,47,47,47,535,535,47,535},
    {536,536,536,536,536,536,536,49,49,536,536,536,536,536,536,536,536,536,536,536,49,49,49,49,49,49,49,49,49,49,49,49,49,48,49,536,536,536,536,536,536,536,49,49,536,49},
    {537,537,50,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,537,50,50,50,50,50,50,537,537,50,537},
    {538,538,538,538,538,538,538,52,52,538,538,538,538,538,538,538,538,538,538,538,52,52,52,52,52,52,52,52,52,52,52,52,51,52,52,538,538,538,538,538,538,538,52,52,538,52},
    {539,539,53,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,539,53,53,53,53,53,53,539,539,54,539},
    {540,540,55,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,540,55,55,55,55,55,55,540,540,540,540},
    {541,541,541,541,541,541,541,57,541,541,541,541,541,541,541,541,541,541,541,541,57,57,57,57,57,57,56,56,56,56,56,56,57,57,57,541,541,541,541,541,541,541,57,57,541,57},
    {542,542,58,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,542,58,58,58,58,58,58,542,542,542,542},
    {543,543,543,543,543,543,543,61,61,543,543,543,543,543,543,543,543,543,543,543,61,61,61,61,59,60,61,61,61,61,61,61,61,61,61,543,543,543,543,543,543,543,61,543,543,61},
    {544,544,62,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,544,62,62,62,62,62,62,544,544,544,544},
    {545,545,545,545,545,545,545,66,66,545,545,545,545,545,545,545,545,545,545,545,66,63,64,65,66,66,66,66,66,66,66,66,66,66,66,545,545,545,545,545,545,545,66,545,545,66},
    {546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,546,67,68,69,70,71,72,546,546,546,546,546,546,546,546,546,546,546,546,546,546},
    {547,547,547,73,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547,547},
    {548,548,548,548,548,548,548,548,548,548,548,74,75,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548,548},
    {549,549,549,549,549,549,549,549,549,549,549,76,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549,549},
    {550,550,550,550,550,550,550,550,550,550,550,78,77,79,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550,550},
    {551,551,551,551,551,551,551,551,551,551,551,551,80,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551,551},
    {552,552,81,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,552,82,83,84,85,86,87,552,552,552,552},
};


int static producciones[88][6]{
    {141,	8,	2,	125,	101,	140},
    {3,	4,	155},
    {2},
    {-1},
    {-1},
    {5,	123,	11,	125,	6},
    {2},
    {-1},
    {7,	12},
    {6,	130},
    {-1	},
    {10,	123,	9},
    {8	},
    {-1	},
    {18	},
    {40	},
    {21	},
    {22	},
    {25	},
    {142},
    {143},
    {144},
    {145},
    {146},
    {13,	101},
    {14},
    {-1},
    {15,	129,	102,	128},
    {14	},
    {-1},
    {17,	101},
    {19},
    {-1},
    {28,	113,	16},
    {20,	28,	128},
    {28,	130},
    {129},
    {152,	8,	125,	28,	151},
    {26,	126,	153},
    {24,	101},
    {127},
    {23,	130	},
    {26,	126,	154},
    {27,	28},
    {127},
    {26,130},
    {29,	30},
    {28,	122},
    {-1	},
    {31,	33},
    {30,	121},
    {-1},
    {33},
    {33,	555	},
    {34,	35},
    {35,	39},
    {-1	},
    {36,	37},
    {35,	108	},
    {35,	109	},
    {-1	},
    {38,	45},
    {37,	110},
    {37,	111},
    {37,	112},
    {-1	},
    {114},
    {116},
    {117},
    {118},
    {119},
    {120},
    {41,	8,	125,	28,	147},
    {44,	42},
    {43},
    {43,	8,	125,	28,	148},
    {44	},
    {42	},
    {150},
    {150,	8,	125,	149},
    {16	},
    {102},
    {103},
    {104},
    {105},
    {106},
    {127,	45,	126	},
    {-1	},

};
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
}

MainWindow::~MainWindow(){
    delete ui;
}

int Relaciona(char c){
    int valor;
    if(c >= 97 && c <= 122)
        return 0;
    if(c >= 65 && c <= 90)
        return 1;
    if(c >= 48 && c <= 57)
        return 2;
    switch(c){
    case '_': valor = 3;
        break;
    case '.': valor = 4;
        break;
    case 'E': valor = 5;
        break;
    case 'e': valor = 6;
        break;
    case '+': valor = 7;
        break;
    case '-': valor = 8;
        break;
    case 39: valor = 9;
        break;
    case '"': valor = 10;
        break;
    case '#': valor = 11;
        break;
    case '*': valor = 12;
        break;
    case '/': valor = 13;
        break;
    case '%': valor = 14;
        break;
    case '&': valor = 15;
        break;
    case '|': valor = 16;
        break;
    case '=': valor = 17;
        break;
    case '>': valor = 18;
        break;
    case '<': valor = 19;
        break;
    case '!': valor = 20;
        break;
    case ':': valor = 21;
        break;
    case ';': valor = 22;
        break;
    case '(': valor = 23;
        break;
    case ')': valor = 24;
        break;
    case '[': valor = 25;
        break;
    case ']': valor = 26;
        break;
    case 32: valor = 28;
        break;
    case '\n': valor = 29;
        break;
    case '\t': valor = 30;
        break;
    case ',': valor = 31;
        break;
    default: valor = 27;
    }
    return valor;
}

QString Tokens;
QString textoA;
QString texto;
QString temp;
void Token(int e){
    temp = QString::number(cT);
    tokenLeido << textoA.trimmed();
    switch(e){
    case 100:
        //Tokens += temp + "\t -> " + textoA.trimmed() + "\t\t -> Palabra Reservada\n";
        textoA.trimmed();
        descripcionToken << "Palabra Reservada";
        cT++;
        break;
    case 101:
        textoA.trimmed();
        descripcionToken << "Identificador";
        cT++;
        break;
    case 102:
        textoA.trimmed();
        descripcionToken << "Constante Entera";
        cT++;
        break;
    case 103:
        textoA.trimmed();
        descripcionToken << "Constante Decimal";
        cT++;
        break;
    case 104:
        textoA.trimmed();
        descripcionToken << "Constante Notación Científica";
        cT++;
        break;
    case 105:
        textoA.trimmed();
        descripcionToken << "Constante Caracter";
        cT++;
        break;
    case 106:
        textoA.trimmed();
        descripcionToken << "Constante String";
        cT++;
        break;
    case 107:
        descripcionToken << "Comentario";
        textoA.trimmed();
        cT++;
        break;
    case 108:
        descripcionToken << "Operador Mas";
        textoA.trimmed();
        cT++;
        break;
    case 109:
        descripcionToken << "Operador Menos";
        textoA.trimmed();
        cT++;
        break;
    case 110:
        descripcionToken << "Operador Multiplicación";
        textoA.trimmed();
        cT++;
        break;
    case 111:
        descripcionToken << "Operador División";
        textoA.trimmed();
        cT++;
        break;
    case 112:
        descripcionToken << "Operador Módulo";
        textoA.trimmed();
        cT++;
        break;
    case 113:
        descripcionToken << "Operador Igual";
        textoA.trimmed();
        cT++;
        break;
    case 114:
        descripcionToken << "Operador Asignación";
        textoA.trimmed();
        cT++;
        break;
    case 115:
        descripcionToken << "Operador NOT";
        textoA.trimmed();
        cT++;
        break;
    case 116:
        descripcionToken << "Operador Diferente";
        textoA.trimmed();
        cT++;
        break;
    case 117:
        descripcionToken << "Operador Menor";
        textoA.trimmed();
        cT++;
        break;
    case 118:
        descripcionToken << "Operador Menor Igual";
        textoA.trimmed();
        cT++;
        break;
    case 119:
        descripcionToken << "Operador Mayor";
        textoA.trimmed();
        cT++;
        break;
    case 120:
        descripcionToken << "Operador Mayor Igual";
        textoA.trimmed();
        cT++;
        break;
    case 121:
        descripcionToken << "Operador AND";
        textoA.trimmed();
        cT++;
        break;
    case 122:
        descripcionToken << "Operador OR";
        textoA.trimmed();
        cT++;
        break;
    case 123:
        descripcionToken << "Punto y Coma";
        textoA.trimmed();
        cT++;
        break;
    case 124:
        descripcionToken << "Punto";
        textoA.trimmed();
        cT++;
        break;
    case 125:
        descripcionToken << "Dos Puntos";
        textoA.trimmed();
        cT++;
        break;
    case 126:
        descripcionToken << "Parentesis que Abre";
        textoA.trimmed();
        cT++;
        break;
    case 127:
        descripcionToken << "Parentesis que Cierra";
        textoA.trimmed();
        cT++;
        break;
    case 128:
        descripcionToken << "Corchete que Abre";
        textoA.trimmed();
        cT++;
        break;
    case 129:
        descripcionToken << "Corchete que Cierra";
        textoA.trimmed();
        cT++;
        break;
    case 130:
        descripcionToken << "Coma";
        textoA.trimmed();
        cT++;
        break;
        /* Palabras Reservadas */
    case 140:
        descripcionToken << "Palabra Reservada HOLYD";
        cT++;
        break;
    case 141:
        descripcionToken << "Palabra Reservada FINISH";
        cT++;
        break;
    case 142:
        descripcionToken << "Palabra Reservada INT";
        cT++;
        break;
    case 143:
        descripcionToken << "Palabra Reservada FLOAT";
        cT++;
        break;
    case 144:
        descripcionToken << "Palabra Reservada CHAR";
        cT++;
        break;
    case 145:
        descripcionToken << "Palabra Reservada STRING";
        cT++;
        break;
    case 146:
        descripcionToken << "Palabra Reservada BOOL";
        cT++;
        break;
    case 147:
        descripcionToken << "Palabra Reservada IF";
        cT++;
        break;
    case 148:
        descripcionToken << "Palabra Reservada ELSEIF";
        cT++;
        break;
    case 149:
        descripcionToken << "Palabra Reservada ELSE";
        cT++;
        break;
    case 150:
        descripcionToken << "Palabra Reservada FI";
        cT++;
        break;
    case 151:
        descripcionToken << "Palabra Reservada WHILE";
        cT++;
        break;
    case 152:
        descripcionToken << "Palabra Reservada ELIHW";
        cT++;
        break;
    case 153:
        descripcionToken << "Palabra Reservada READ";
        cT++;
        break;
    case 154:
        descripcionToken << "Palabra Reservada WRITE";
        cT++;
        break;
    case 155:
        descripcionToken << "Palabra Reservada DEFINE";
        cT++;
        break;
    }

}

QString errores;
void Errores(int e){



    temp = QString::number(cE);
    QMessageBox error;
    switch(e){
    case 500:
        descripcionError << "Error 500: No corresponde al lenguaje..";
        errorLeido << textoA.trimmed();
        cE++;
        break;
    case 501:
        descripcionError << "Error 501: Se esperaba digito despues de \".\"";
        errorLeido << textoA.trimmed();
        cE++;
        break;
    case 502:
        descripcionError << "Error 502: Se esperaba digito despues de \"E\", \"e\"";
        errorLeido << textoA.trimmed();
        cE++;
        break;
    case 503:
        descripcionError << "Error 503: Se esperaba digito despues de \"+\", \"-\"";
        errorLeido << textoA.trimmed();
        cE++;
        break;
    case 504:
        descripcionError << "Error 504: Caracter mal formado, falto un caracter";
        errorLeido << textoA.trimmed();
        cE++;
        break;
    case 505:
        descripcionError << "Error 505: Caracter mal formado, falto una \'";
        errorLeido << textoA.trimmed();
        cE++;
        break;
    case 506:
        descripcionError << "Error 506: Operador AND mal formado";
        errorLeido << textoA.trimmed();
        cE++;
        break;
    case 507:
        descripcionError << "Error 507: Operador OR mal formado";
        errorLeido << textoA.trimmed();
        cE++;
        break;
        /* Errores Sintacticos */
    case 508:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 509:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 510:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 511:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 512:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 513:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 514:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 515:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 516:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 517:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 518:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 519:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 520:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 521:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 522:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 523:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 524:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 525:
        descripcionError << "Error ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 526:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 527:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 528:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 529:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 530:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 531:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 532:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 533:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 534:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 535:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 536:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 537:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 538:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 539:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 540:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 541:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 542:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 543:
        descripcionError << "Error  ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
    case 544:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 545:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 546:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 547:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 548:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 549:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 550:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 551:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;
    case 552:
        descripcionError << "Error al intentar hacer la operación: ";
        errorLeido << textoA.trimmed();
        error.setText("Error, por favor verifica que todo esta bien en tu sintaxis");
        error.exec();
        cE++;
        break;

    default:
       // descripcionError << "Revisa tu Sintaxis de nuevo.";
       // errorLeido << textoA.trimmed();
       // cE++;
        break;
    }

}

/* Método para verificar que se trata de una palabra reservada */
int evaluaPalabraReservada(){
    int conta = 0;
    std::string cadenaStd = textoA.toStdString();
    for(int i = 0; i < textoA.length(); i++){
        char car = cadenaStd[i];
        if(car == '\n' || car == '\t' || car == 32)
            conta++;
    }
    QString temp = textoA.mid(conta, textoA.length());

    if(textoA == "holyd" || temp == "holyd")
        return 140;
    if(textoA == "finish" || temp == "finish")
        return 141;
    if(textoA == "int" || temp == "int")
        return 142;
    if(textoA == "integer" || temp == "integer")
        return 142;
    if(textoA == "float" || temp == "float")
        return 143;
    if(textoA == "char" || temp == "char")
        return 144;
    if(textoA == "string" || temp == "string")
        return 145;
    if(textoA == "bool" || temp == "bool")
        return 146;
    if(textoA == "if" || temp == "if")
        return 147;
    if(textoA == "elseif" || temp == "elseif")
        return 148;
    if(textoA == "else" || temp == "else")
        return 149;
    if(textoA == "fi" || temp == "fi")
        return 150;
    if(textoA == "while" || temp == "while")
        return 151;
    if(textoA == "elihw" || temp == "elihw")
        return 152;
    if(textoA == "read" || temp == "read")
        return 153;
    if(textoA == "write" || temp == "write")
        return 154;
    if(textoA == "define" || temp == "define")
        return 155;

    /*if((textoA=="holyd" || temp=="holyd")  || (textoA=="finish" || temp=="finish")  ||
       (textoA=="int" || temp=="int") ||  (textoA=="float" || temp=="float") ||
       (textoA=="char" || temp=="char") || (textoA=="string" || temp=="string") ||
       (textoA=="bool" || temp=="bool") || (textoA=="if" || temp=="if") || (textoA=="elseif" || temp=="elseif")
       || (textoA=="fi" || temp=="fi") || (textoA=="while" || temp=="while")
       || (textoA=="elihw" || temp=="elihw") || (textoA=="read" || temp=="read")
       || (textoA=="write" || temp=="write") || (textoA=="define" || temp=="define")){
        return 100;
    }*/
    return 101;
}

int Analiza(QString cadena){
    std::string cadenaStd = cadena.toStdString();
    edo = 0;
    int col;
    char car=cadenaStd[0];
    textoA = "";

    int numero = 1;
    while(edo <= 32){
        col = Relaciona(car);
        edo = M[edo][col];
        if(edo==108)
            textoA.append('+');
        if(edo==109)
            textoA.append('-');
        if(edo==110)
            textoA.append('*');
        if(edo==111)
            textoA.append('/');
        if(edo==124)
            textoA.append('.');
        if(edo==128)
            textoA.append('[');
        if(edo==129)
            textoA.append(']');
        if(edo==126)
            textoA.append('(');
        if(edo==127)
            textoA.append(')');
        if(edo==123)
            textoA.append(';');
        if(edo==125)
            textoA.append(':');
        if(edo==112)
            textoA.append('%');
        if(edo==121)
            textoA.append("&");
        if(edo==122)
            textoA.append("|");
        if(edo==130)
            textoA.append(",");

        if(edo < 100 || edo >= 500)
            textoA.append(car);

        if(edo == 103 && (car == 'e' || car == 'E')){
            if(car == 'e')
                textoA.append('e');
            if(car == 'E')
                textoA.append('E');
            edo = 6;
        }

        if(edo==114 && car=='='){
            textoA.append('=');
            edo=114;
        }

        if(edo==120 && car=='='){
            textoA.append('=');
            edo=120;
        }

        if(edo==118 && car=='='){
            textoA.append('=');
            edo=118;
        }

        if(edo==116 && car=='='){
            textoA.append('=');
            edo=116;
        }

        if(edo==115 && car=='='){
            textoA.append('=');
            edo=116;
        }

        if(edo==113 && car=='='){
            textoA.append('=');
            edo=113;
        }

        if(edo==107 && car=='#'){
            textoA.append('#');
            edo=107;
        }

        if(edo==9 && car!=39){
            textoA.append(car);
            edo=10;
        }else if(edo==9 && car!=39){
            textoA.append(car);
            edo=105;
        }

        if(edo==105 && car==39){
            textoA.append(car);
            edo=105;
        }

        if(edo==100)
            edo = evaluaPalabraReservada();

        if(edo==106 && car=='"'){
            textoA.append(car);
            edo=106;
        }

        car = cadenaStd[numero];

        numero++;
    }
    if(textoA=="&&" || textoA.contains("&&")){
        edo = 121;
    }else if (textoA.contains("&")) {
        edo = 506;
    }

    if(textoA=="||" || textoA.contains("||")){
        edo = 122;
    }else if (textoA.contains("|")) {
        edo = 507;
    }
    if(textoA=="^"){
        edo = 94;
    }



    int conta=0;
    std::string cadenaStd2 = textoA.toStdString();
    for(int i = 0; i < textoA.length(); i++){
        char car = cadenaStd[i];
        if(car==' ' || car=='\t' || car==32 )
            conta++;
    }

    int longitud = textoA.length();
    texto=texto.remove(0,longitud);
    if(conta!=0)
        textoA.remove(0,conta);

    if(edo>=100 && edo<=199)
        Token(edo);
    else
        Errores(edo);

    return edo;
}
QString descripcion = "";
int relacionaGramatica(int estado){


    switch(estado){

    case 100: break;
    case 140: return 0;  //holyd
    case 155: return 1;  //define
    case 101: return 2;  //id
    case 147: return 3;  //if
    case 151: return 4;  //while
    case 153: return 5;  //read
    case 154: return 6;  //write
    case 130: return 7;  //,
    case 125: return 8;  //:
    case 141: return 9;  //finish
    case 152: return 10; //elihw
    case 148: return 11; //elseif
    case 149: return 12; //else
    case 150: return 13; //fi
    case 142: return 14; //int
    case 143: return 15; //float
    case 144: return 16; //char
    case 145: return 17; //string
    case 146: return 18; //bool
    case 128: return 19; //[
    case 114: return 20; //=
    case 110: return 21; //*
    case 111: return 22; //division
    case 112: return 23; //%
    case 108: return 24; //+
    case 109: return 25; //-
    case 113: return 26; //==
    case 116: return 27; /*!=*/
    case 117: return 28; //<
    case 118: return 29; //<=
    case 119: return 30; //>
    case 120: return 31; //>=
    case 121: return 32; //&&
    case 122: return 33; //||
    case 129: return 34; //]
    case 555: return 35; /*!*/
    case 102: return 36; //cteentera
    case 103: return 37; //ctereal
    case 104: return 38; //ctenotacion
    case 105: return 39; //ctechar
    case 106: return 40; //ctestring
    case 126: return 41; //(
    case 127: return 42; //)
    case 123: return 45; /*;*/
        /* $ */
    default: return 44;
    }
}

void llenarPilaProduccion(int fila){
    cout<<"Logre entrar aqui de milagro"<<endl;
    for(int i = 0; i < 6; i++){
        if(producciones[fila][i] != 0){
            pilaEjecucion.push(producciones[fila][i]);
            cout<<"WACHA MUCHCAHCO----------------------------------"<<fila<<"-------------"<<pilaEjecucion.top()<<endl;

        }
    }
}

QString evaluaElemento(int token){
    switch(token){
    case -1:
        return "ε";
        break;
    case 1:
        return "PROGRAMA";
        break;
    case 2:
        return "DECLARA";
        break;
    case 3:
        return "DECLARA3";
        break;
    case 4:
        return "DECLARA2";
        break;
    case 5:
        return "DECLARA4";
        break;
    case 6:
        return "ID_DECLARA";
        break;
    case 7:
        return "ID_DECLARA2";
        break;
    case 8:
        return "ESTATUTOS";
        break;
    case 9:
        return "ESTATUTOS2";
        break;
    case 10:
        return "ESTATUTOS3";
        break;
    case 11:
        return "TIPOv ";//xd
        break;
    case 12:
        return "ID_DIM";
        break;
    case 13:
        return "ID_DIM2";
        break;
    case 14:
        return "DIM";
        break;
    case 15:
        return "DIM2";
        break;
    case 16:
        return "ASIG";
        break;
    case 17:
        return "ASIG2";
        break;
    case 18:
        return "EST_ASIG";
        break;
    case 19:
        return "DIM_ASIG";
        break;
    case 20:
        return "DIM_ASIG2";
        break;
    case 21:
        return "EST_WHILE";
        break;
    case 22:
        return "EST_READ";
        break;
    case 23:
        return "EST_IDR";
        break;
    case 24:
        return "EST_IDR2";
        break;
    case 25:
        return "EST_WRITE";
        break;
    case 26:
        return "EST_EXPRW";
        break;
    case 27:
        return "EST_EXPRW2";
        break;
    case 28:
        return "EXPR";
        break;
    case 29:
        return "EXPR6";
        break;
    case 30:
        return "EXPR2";
        break;
    case 31:
        return "EXPR7";
        break;
    case 32:
        return "EXPR3";
        break;
    case 33:
        return "EXPR4";
        break;
    case 34:
        return "EXPR8";
        break;
    case 35:
        return "EXPR5";
        break;
    case 36:
        return "EXPR9";
        break;
    case 37:
        return "TERM";
        break;
    case 38:
        return "TERM2";
        break;
    case 39:
        return "OPREL";
        break;
    case 40:
        return "EST_IF";
        break;
    case 41:
        return "EST_IF2";
        break;
    case 42:
        return "ELSEIF";
        break;
    case 43:
        return "ELSEIF2";
        break;
    case 44:
        return "ELSE";
        break;
    case 45:
        return "FACT";
        break;
    case 46:
        return "$";
        break;
    case 94: return "$";
    case 101: return "id";
    case 102: return "cte_entera";
    case 103: return "cte_real";
    case 104: return "cte_notacion";
    case 105: return "cte_char";
    case 106: return "cte_string";
    case 108: return "+";
    case 109: return "-";
    case 110: return "*";
    case 111: return "/";
    case 112: return "%";
    case 113: return "==";
    case 114: return "=";
    case 115: return "!";
    case 116: return "!=";
    case 117: return "<";
    case 118: return "<=";
    case 119: return ">";
    case 120: return ">=";
    case 121: return "&&";
    case 122: return "||";
    case 123: return ";";
    case 125: return ":";
    case 126: return "(";
    case 127: return ")";
    case 128: return "[";
    case 129: return "]";
    case 130: return ",";
    case 140: return "holyd";
    case 141: return "finish";
    case 142: return "integer";
    case 143: return "float";
    case 144: return "char";
    case 145: return "string";
    case 146: return "bool";
    case 147: return "if";
    case 148: return "elseif";
    case 149: return "else";
    case 150: return "fi";
    case 151: return "while";
    case 152: return "elihw";
    case 153: return "read";
    case 154: return "write";
    case 155: return "define";
    default: return "Desconocido";
    }

}

QString pasosPila;
void imprimePila(){
    for(int i = 0; i < pilaEjecucion.size(); i++){
        QString ele = evaluaElemento(pilaEjecucion.at(i));
        cout<<"EL ELEMENTO QUE SE SACO PA IMPRIMIR ES: "<<ele.toStdString()<<endl;
        pasosPila += ele + " ";
    }
    cout<<"Aqui termina la ejecución de imprimir pila"<<endl;
    pasosPila += "\n";
}

void ConstruyeGramatica(){
    int token = 0, edoMP = 0, colMP = 0, filaMP = 0;
    while(!pilaEjecucion.empty()){
        pilaEjecucion.pop();
    }

    pilaEjecucion.push('^');
    pilaEjecucion.push(1);
    bool quieroToken = true, llena = true;
    imprimePila();

    while(edoMP < 500 && token < 500 && !pilaEjecucion.empty()){
        if(quieroToken){
            token = Analiza(texto);
            cout<<"////////////////////////////////////////////////////////////////////////////////////"<<endl;
            cout<<"EL TEXTO PARA REALIZAR LAS OPERACIONES ES: "<<textoA.toStdString()<<endl;
            quieroToken = false;
            if(textoA=="^")
                goto fina;
        }
        cout << token << endl;
        cout<<"MI TOKEN: "<<token<<" MI PILITA: "<<pilaEjecucion.top()<<endl;
        /*if(token >= 500)
            break;
        if(token==141 && pilaEjecucion.top()==41){
            QMessageBox msgBox;
            msgBox.setText("Hay errores en la sintaxis");
            msgBox.setText("Hay errores en la sintaxis porque se esperaba un else");
            cT--;
            msgBox.exec();

            break;
        }*/


        if(llena){
            //cout<<"EL TEXTO QUE TIENE ACTUALMENTE ES: "<<texto.toStdString()<<endl;
            cout<<"ESTAMOS DENTRO DE LLENA:"<<endl;
            colMP = relacionaGramatica(token);
            filaMP = pilaEjecucion.top()-1;
            cout << colMP << " " << filaMP << endl;
            pilaEjecucion.pop();
            edoMP = matrizPredictiva[filaMP][colMP]-1;
            cout<<"EL TOKEN FUE "<<token<<endl;
            cout<<"FILA: "<<filaMP<<endl;
            cout<<" COLUMNA "<<colMP<<endl;
            cout<<"EDO: "<<edoMP<<endl;
            if(edoMP>=508){
                Errores(edoMP);
                cT--;
                break;
            }
            llenarPilaProduccion(edoMP);
            llena = false;
            cout<<"ANTES DE MANDAR LA IMPRESION EL ESTADO EN LA MATRIZ ES DE: "<<edoMP<<endl;
            imprimePila();
        }
        if(pilaEjecucion.top() >= 100){
            cout << "Token " << token << " Token en la pila " << pilaEjecucion.top()<<endl;
            if(pilaEjecucion.top() == token){
                pilaEjecucion.pop();
                if(texto!=""){
                    quieroToken = true;
                    cout<<"Efectivamente...el texto esta vacio"<<endl;
                }
                imprimePila();
            }else{
                cout<<"SE ESTA EVALUANDO ESTE ELEMENTO"<<endl;
                QString tr = evaluaElemento(token);
                QString tp = evaluaElemento(pilaEjecucion.top());
                QMessageBox msgBox;

                QMessageBox msjito2;
                // msjito2.setText("ERROR");
                // msjito2.setText("Algo salio mal, verifica tu sintaxis");
                // msjito2.exec();
                msgBox.setText("Hay errores en la sintaxis");
                msgBox.setText("Hay errores en la sintaxis porque se esperaba un "+tp+" y se recibio un "+tr);
                msgBox.exec();
                break;
            }
        }else{
            if(edoMP>500){
                Errores(edoMP+1);
            }else{
                if(pilaEjecucion.top() > 0 && pilaEjecucion.top() <= 46){
                    cout<<"ACABAMOS DE PONER LLENA EN POSITIVO XD"<<endl;
                    llena = true;
                }
                if(pilaEjecucion.top() == -1){
                    cout<<"LA PILA DE EJECUCIÓN ES: "<<pilaEjecucion.top()<<endl;
                    pilaEjecucion.pop();
                    imprimePila();
                    cout<<"LA PILA DE EJECUCIÓN ES: "<<pilaEjecucion.top()<<endl;
                    cout<<"IMPRESION NEGATIVA....CHALE"<<endl;
                }
                fina:
                if(pilaEjecucion.top() == '^'){
                    pilaEjecucion.pop();
                    imprimePila();
                    cout<<"IMPRESION $$$$$$"<<endl;
                    QMessageBox msgBox;
                    msgBox.setText("Se llego al final de la entrada y no hay errores");
                    msgBox.exec();
                    break;
                }
            }
        }

    }
}








void MainWindow::on_cleanTables_clicked(){
    /*ui->tableTokens->clearContents();
    ui->tableTokens->model()->removeRows(0, ui->tableTokens->rowCount());
    ui->tableErrores->clearContents();
    ui->tableErrores->model()->removeRows(0, ui->tableErrores->rowCount());
    ui->textoAnalizar->setPlainText("");
    //ui->Token_2->setPlainText("");
    ui->tableTokens->setRowCount(0);
    ui->tableErrores->setRowCount(0);
    tokenLeido.clear();
    errorLeido.clear();
    descripcionError.clear();
    descripcionToken.clear();

    ui->tableTokens->clearContents();
    ui->tableTokens->model()->removeRows(0, ui->tableTokens->rowCount());

    */
    ui->tableTokens->setRowCount(0);
    ui->tableErrores->setRowCount(0);
    ui->textoAnalizar->setPlainText("");
    pilaEjecucion.clear();
}



void MainWindow::on_actionRun_triggered()
{
    //Tokens = "";
    //errores = "";
    cT = 0;
    cE = 0;
    texto = "";
    texto=ui->textoAnalizar->toPlainText();
    texto.append("^");
    //while(texto!="")
    //Analiza(texto);
    ConstruyeGramatica();
    if(errores != "")
        QMessageBox::about(this, "Sintaxis", "Se encontraron algunos errores lexicos o sintacticos");

    /* Tabla TOKENS */
    ui->tableTokens->setColumnCount(2);
    QStringList t;
    t << "Token" << "Descripción Token";
    ui->tableTokens->setHorizontalHeaderLabels(t);
    ui->tableTokens->setColumnWidth(0, 100);
    ui->tableTokens->setColumnWidth(1, 420);
    for(int c = 0; c < cT; c++){
        ui->tableTokens->insertRow(ui->tableTokens->rowCount());
        ui->tableTokens->setItem(ui->tableTokens->rowCount()-1, 0, new QTableWidgetItem(tokenLeido[c]));
        ui->tableTokens->setItem(ui->tableTokens->rowCount()-1, 1, new QTableWidgetItem(descripcionToken[c]));
    }

    /* Tabla Errores */
    ui->tableErrores->setColumnCount(2);
    QStringList e;
    e << "Error" << "Descripción Error";
    ui->tableErrores->setHorizontalHeaderLabels(e);
    ui->tableErrores->setColumnWidth(0, 100);
    ui->tableErrores->setColumnWidth(1, 420);
    for(int c = 0; c < cE; c++){
        ui->tableErrores->insertRow(ui->tableErrores->rowCount());
        ui->tableErrores->setItem(ui->tableErrores->rowCount()-1, 0, new QTableWidgetItem("ERROR"));
        ui->tableErrores->setItem(ui->tableErrores->rowCount()-1, 1, new QTableWidgetItem(descripcionError[c]));
    }
    cout<<"---------LA PILA DE EJECUCIÓN FINAL FUE LA SIGUIENTE--------"<<endl;
    cout<<pasosPila.toStdString()<<endl;

    ui->txtPila->appendPlainText(pasosPila);
}

void MainWindow::on_actionRead_triggered()
{
    QString ruta=QFileDialog::getOpenFileName(
                this,tr("Abrir archivo"),"C:\\","ISC Files (*.isc)");
    if(ruta!=""){
        QFile inputFile(ruta);
        if (inputFile.open(QIODevice::ReadOnly)){
            QTextStream in(&inputFile);
            while (!in.atEnd()){
                QString line = in.readLine();
                ui->textoAnalizar->appendPlainText(line);
            }
            inputFile.close();
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Guardar como..."), "",
                                                    tr("ISC Files (*.isc)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Imposible realizar la operación"),
                                     file.errorString());
            return;
        }
    }
    QFile file(fileName+".isc");
    if ( file.open(QIODevice::ReadWrite) ){
        QTextStream stream( &file );
        stream <<ui->textoAnalizar->toPlainText()<<endl;
    }
}

void MainWindow::on_actionClean_triggered()
{
    ui->tableTokens->setRowCount(0);
    ui->tableErrores->setRowCount(0);
    ui->textoAnalizar->setPlainText("");
    ui->txtPila->setPlainText("");
}

void MainWindow::on_actionExit_triggered()
{
    close();
}


void MainWindow::on_actionInformaci_n_triggered()
{
    QMessageBox mensajito;
    mensajito.setText("Este programa es un analizador Sintáctico y Léxico que fue creado para la materia de Lenguajes y "
                      "Autómatas I de la carrera de Ing. Sistemas Computacionales. Creado por Carlos Alberto Carrete Gómez");
    mensajito.exec();
}
