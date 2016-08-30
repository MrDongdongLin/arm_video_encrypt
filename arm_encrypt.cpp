#include "arm_encrypt.h"
#include <fstream>
using namespace std;

#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>

#include <QGridLayout>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    X1=-581;X2=421;X3=233;X4=-347;X5=-21;X6=13;X7=-63;X8=-603;

    EncryptCatInit();

    m_image_path           = new QLabel(this);
    m_key_info             = new QLabel(this);
    m_label_plain_image    = new QLabel(this);
    m_label_cipher_image   = new QLabel(this);
    m_button_open_image    = new QPushButton(this);
    m_button_encrypt_image = new QPushButton(this);
    m_button_decrypt_image = new QPushButton(this);

//    m_label_plain_image->setGeometry(0, 0, 256, 256);
//    m_label_cipher_image->setGeometry(0, 0, 256, 256);
    m_image_path->setText(tr("The image path"));
    m_key_info->setText(tr("Encrypt Keys"));
    m_label_plain_image->setText(tr("Plain Image"));
    m_label_cipher_image->setText(tr("Cipher Image"));
    m_button_open_image->setText(tr("Browse"));
    m_button_encrypt_image->setText(tr("Encrypt Image"));
    m_button_decrypt_image->setText(tr("Decrypt Image"));
    m_image_path->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_key_info->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(m_label_plain_image, 0, 0);
    mainLayout->addWidget(m_label_cipher_image, 0, 1);
    mainLayout->addWidget(m_image_path, 1, 0);
    mainLayout->addWidget(m_button_open_image, 1, 1);
    mainLayout->addWidget(m_key_info, 2, 0);
    mainLayout->addWidget(m_button_encrypt_image, 2, 1);
    mainLayout->addWidget(m_button_decrypt_image, 3, 1);

    connect(m_button_open_image, SIGNAL(clicked(bool)), this, SLOT(openImage()));
    connect(m_button_encrypt_image, SIGNAL(clicked(bool)), this, SLOT(encryptImage()));
    connect(m_button_decrypt_image, SIGNAL(clicked(bool)), this, SLOT(decryptImage()));
}

Dialog::~Dialog()
{

}

void Dialog::EncryptCatInit(){
    int  index = 0;
    int a,b,c,d,e,f,g,h;
    for (a = 0; a < 4; a ++){
    for (b = 0; b < 4; b ++){
    for (c = 0; c < 4; c ++){
    for (d = 0; d < 4; d ++){
    for (e = 0; e < 4; e ++){
    for (f = 0; f < 4; f ++){
    for (g = 0; g < 4; g ++){
    for (h = 0; h < 4; h ++){
        table[index++] =
                (((a11*a +  a12*b + a13*c + a14*d + a15*e + a16*f + a17*g + a18*h) % 4) << 14) +
                (((a21*a +  a22*b + a23*c + a24*d + a25*e + a26*f + a27*g + a28*h) % 4) << 12) +
                (((a31*a +  a32*b + a33*c + a34*d + a35*e + a36*f + a37*g + a38*h) % 4) << 10) +
                (((a41*a +  a42*b + a43*c + a44*d + a45*e + a46*f + a47*g + a48*h) % 4) << 8) +
                (((a51*a +  a52*b + a53*c + a54*d + a55*e + a56*f + a57*g + a58*h) % 4) << 6) +
                (((a61*a +  a62*b + a63*c + a64*d + a65*e + a66*f + a67*g + a68*h) % 4) << 4) +
                (((a71*a +  a72*b + a73*c + a74*d + a75*e + a76*f + a77*g + a78*h) % 4) << 2) +
                (( a81*a +  a82*b + a83*c + a84*d + a85*e + a86*f + a87*g + a88*h) % 4);
    }}}}}}}}
}

void Dialog::openImage(){
    QString path_img = QFileDialog::getOpenFileName(this,
                                            tr("Select an image"),
                                            "",
                                            tr("Images (*.png *.bmp)")
                                            );
    if(path_img.isEmpty()) return;
    else{
        QImage *img = new QImage;
        if(!(img->load(path_img))){
            QMessageBox::information(this,
                                     tr("Error Box"),
                                     tr("Open image failed!")
                                     );
            delete img;
            return;
        }
        m_label_plain_image->setPixmap(QPixmap::fromImage(*img));
    }
    m_image_path->setText(path_img);
    is_readImg = true;
}

void Dialog::encryptImage(){
    if(!(is_readImg)){
        QMessageBox::information(this, "Error Box", "Please read an image ahead!");
        return;
    }
    QString path_img = m_image_path->text();
    QImage *pimage = new QImage(path_img);
    int width = pimage->width();
    int height = pimage->height();
    int i, j, index;
    /*--------------- copy plain image ----------------*/
    uchar *r = new uchar[height*width];
    uchar *g = new uchar[height*width];
    uchar *b = new uchar[height*width];
    memset(r, 0, sizeof(uchar));
    memset(g, 0, sizeof(uchar));
    memset(b, 0, sizeof(uchar));
    QRgb value;
    for(i=0; i<height; i++){
        for(j=0; j<width; j++){
            value = pimage->pixel(i, j);     // row first;
            r[i*width+j] = qRed(value);
            g[i*width+j] = qGreen(value);
            b[i*width+j] = qBlue(value);
        }
    }
    /*----------- pixel position scrambling ------------*/
    uchar *r_encrypt = new uchar[height*width];
    uchar *g_encrypt = new uchar[height*width];
    uchar *b_encrypt = new uchar[height*width];
    memset(r_encrypt, 0, sizeof(uchar));
    memset(g_encrypt, 0, sizeof(uchar));
    memset(b_encrypt, 0, sizeof(uchar));
    for(index=0; index<65536; index++){
        r_encrypt[table[index]] = r[index];
        g_encrypt[table[index]] = g[index];
        b_encrypt[table[index]] = b[index];
    }
    delete [] r;
    delete [] g;
    delete [] b;
    /*------------- pixel value encryption -------------*/
    float x1,x2,x3,x4,x5,x6,x7,x8;
    ofstream fp_rgb;
    fp_rgb.open("../data/rgb_encrypt.txt");
    for(index=0; index<65536; index++){
        r_encrypt[index] = r_encrypt[index] ^ ((int)X1 % 256);
        g_encrypt[index] = g_encrypt[index] ^ ((int)X2 % 256);
        b_encrypt[index] = b_encrypt[index] ^ ((int)X3 % 256);
        fp_rgb << (int)r_encrypt[index] << "\t" << (int)g_encrypt[index] << "\t" << (int)b_encrypt[index] << endl;
        x1 = A11*X1 +A12*X2 +A13*X3 +A14*X4 +A15*X5 +A16*X6 +A17*X7 +A18*X8;
        x2 = A21*X1 +A22*X2 +A23*X3 +A24*X4 +A25*X5 +A26*X6 +A27*X7 +A28*X8;
        x3 = A31*X1 +A32*X2 +A33*X3 +A34*X4 +A35*X5 +A36*X6 +A37*X7 +A38*X8;
        x4 = A41*r_encrypt[index] +A42*g_encrypt[index] +A43*b_encrypt[index] +A44*X4 +A45*X5 +A46*X6 +A47*X7 +A48*X8
                +(B1*r_encrypt[index])%E1;
        x5 = A51*r_encrypt[index] +A52*g_encrypt[index] +A53*b_encrypt[index] +A54*X4 +A55*X5 +A56*X6 +A57*X7 +A58*X8
                +(B2*g_encrypt[index])%E2;
        x6 = A61*r_encrypt[index] +A62*g_encrypt[index] +A63*b_encrypt[index] +A64*X4 +A65*X5 +A66*X6 +A67*X7 +A68*X8
                +(B3*b_encrypt[index])%E3;
        x7 = A71*r_encrypt[index] +A72*g_encrypt[index] +A73*b_encrypt[index] +A74*X4 +A75*X5 +A76*X6 +A77*X7 +A78*X8
                +(B1*r_encrypt[index])%E1 +(B2*g_encrypt[index])%E2;
        x8 = A81*r_encrypt[index] +A82*g_encrypt[index] +A83*b_encrypt[index] +A84*X4 +A85*X5 +A86*X6 +A87*X7 +A88*X8
                +(B2*g_encrypt[index])%E2 +(B3*b_encrypt[index])%E3;
        X1=x1, X2=x2, X3=x3, X4=x4, X5=x5, X6=x6, X7=x7, X8=x8;
    }
    fp_rgb.close();
    /*------------------ show image -------------------*/
    QImage *cimage = new QImage(width, height, QImage::Format_RGB888);
    for(i=0; i<height; i++){
        for(j=0; j<width; j++){
            value = qRgb(r_encrypt[i*width+j], g_encrypt[i*width+j], b_encrypt[i*width+j]);
            cimage->setPixel(i, j, value);
        }
    }
    m_label_cipher_image->setPixmap(QPixmap::fromImage(*cimage));

    QString c_path_img = QFileDialog::getSaveFileName(this,
                                            tr("Save image"),
                                            "",
                                            tr("Images (*.png *.bmp)")
                                            );
    cimage->save(c_path_img,"bmp");
    m_image_path->setText(c_path_img);

    is_encrypt = true;
    delete pimage;
    delete cimage;
    delete [] r_encrypt;
    delete [] g_encrypt;
    delete [] b_encrypt;
}

void Dialog::decryptImage(){
    if(!is_encrypt){
        QMessageBox::information(this, "Error Box", "Please encrypt the image ahead!");
        return;
    }
    QString path_img = m_image_path->text();
    QImage *cimage = new QImage(path_img);
    int width = cimage->width();
    int height = cimage->height();
    int i, j, index;
    /*--------------- copy plain image ----------------*/
    uchar *r_encrypt = new uchar[height*width];
    uchar *g_encrypt = new uchar[height*width];
    uchar *b_encrypt = new uchar[height*width];
    memset(r_encrypt, 0, sizeof(uchar));
    memset(g_encrypt, 0, sizeof(uchar));
    memset(b_encrypt, 0, sizeof(uchar));
    QRgb value;
    for(i=0; i<height; i++){
        for(j=0; j<width; j++){
            value = cimage->pixel(i, j);     // row first;
            r_encrypt[i*width+j] = qRed(value);
            g_encrypt[i*width+j] = qGreen(value);
            b_encrypt[i*width+j] = qBlue(value);
        }
    }
    /*------------- pixel value decryption -------------*/
    float x1,x2,x3,x4,x5,x6,x7,x8;
    for(index=0; index<65536; index++){
        x1 = A11*X1 +A12*X2 +A13*X3 +A14*X4 +A15*X5 +A16*X6 +A17*X7 +A18*X8;
        x2 = A21*X1 +A22*X2 +A23*X3 +A24*X4 +A25*X5 +A26*X6 +A27*X7 +A28*X8;
        x3 = A31*X1 +A32*X2 +A33*X3 +A34*X4 +A35*X5 +A36*X6 +A37*X7 +A38*X8;
        x4 = A41*r_encrypt[index] +A42*g_encrypt[index] +A43*b_encrypt[index] +A44*X4 +A45*X5 +A46*X6 +A47*X7 +A48*X8
                +(B1*r_encrypt[index])%E1;
        x5 = A51*r_encrypt[index] +A52*g_encrypt[index] +A53*b_encrypt[index] +A54*X4 +A55*X5 +A56*X6 +A57*X7 +A58*X8
                +(B2*g_encrypt[index])%E2;
        x6 = A61*r_encrypt[index] +A62*g_encrypt[index] +A63*b_encrypt[index] +A64*X4 +A65*X5 +A66*X6 +A67*X7 +A68*X8
                +(B3*b_encrypt[index])%E3;
        x7 = A71*r_encrypt[index] +A72*g_encrypt[index] +A73*b_encrypt[index] +A74*X4 +A75*X5 +A76*X6 +A77*X7 +A78*X8
                +(B1*r_encrypt[index])%E1 +(B2*g_encrypt[index])%E2;
        x8 = A81*r_encrypt[index] +A82*g_encrypt[index] +A83*b_encrypt[index] +A84*X4 +A85*X5 +A86*X6 +A87*X7 +A88*X8
                +(B2*g_encrypt[index])%E2 +(B3*b_encrypt[index])%E3;
        r_encrypt[index] = r_encrypt[index] ^ ((int)X1 % 256);
        g_encrypt[index] = g_encrypt[index] ^ ((int)X2 % 256);
        b_encrypt[index] = b_encrypt[index] ^ ((int)X3 % 256);
        X1=x1, X2=x2, X3=x3, X4=x4, X5=x5, X6=x6, X7=x7, X8=x8;
    }
    /*--------- anti pixel position scrambling ----------*/
    uchar *r_decrypt = new uchar[height*width];
    uchar *g_decrypt = new uchar[height*width];
    uchar *b_decrypt = new uchar[height*width];
    memset(r_decrypt, 0, sizeof(uchar));
    memset(g_decrypt, 0, sizeof(uchar));
    memset(b_decrypt, 0, sizeof(uchar));
    for(index=0; index<65536; index++){
        r_decrypt[index] = r_encrypt[table[index]];
        g_decrypt[index] = g_encrypt[table[index]];
        b_decrypt[index] = b_encrypt[table[index]];
    }
    delete [] r_encrypt;
    delete [] g_encrypt;
    delete [] b_encrypt;
    /*------------------ show image -------------------*/
    QImage *aimage = new QImage(width, height, QImage::Format_RGB888);
    for(i=0; i<height; i++){
        for(j=0; j<width; j++){
            value = qRgb(r_decrypt[i*width+j], g_decrypt[i*width+j], b_decrypt[i*width+j]);
            aimage->setPixel(i, j, value);
        }
    }
    delete [] r_decrypt;
    delete [] g_decrypt;
    delete [] b_decrypt;
    m_label_cipher_image->setPixmap(QPixmap::fromImage(*aimage));

    QString a_path_img = QFileDialog::getSaveFileName(this,
                                            tr("Save image"),
                                            "",
                                            tr("Images (*.png *.bmp)")
                                            );
    aimage->save(a_path_img,"bmp");
    m_image_path->setText(a_path_img);

    delete aimage;
}
