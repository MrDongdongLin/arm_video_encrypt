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
    B1=40000 , B2=50000 , B3 = 30000;
    E1=1000000000,E2=1200000000,E3=750000000;

    a11 = 5990537,  a12 = 9868413,  a13 = 226236,  a14 = 4794846,  a15 =  22802340,  a16 = 143956428,  a17 = 24265311,  a18 = 234878586;
    a21 = 103377,   a22 = 170555,   a23 =3300 ,    a24 = 72240,    a25 = 345054,     a26 = 2207175,    a27 = 389517,    a28 = 3597285;
    a31 = 9064191,  a32 = 14931951, a33 = 341837,  a34 = 7246707,  a35 = 34463589,   a36 = 217599315,  a37 = 36692373,  a38 = 355030845;
    a41 = 4251336,  a42 = 7002429,  a43 = 162735,  a44 = 3440882,  a45 = 16357935,   a46 = 103167336,  a47 = 17326695,  a48 = 168341529;
    a51 = 1743033,  a52 =2870931,   a53 = 66858,   a54 = 1412607,  a55 = 6715337,    a56 = 42346893,   a57 = 7108446,   a58 = 69099612;
    a61 = 6081,     a62 = 10086,    a63 = 195,     a64 = 2703,     a65 = 13386,      a66 = 88019,      a67 = 16818,     a68 = 143121;
    a71 = 15774 ,   a72 = 25926,    a73 = 939,     a74 = 16023,    a75 = 76080,      a76 = 467016,     a77 = 70400,     a78 = 763779;
    a81 = 23712,    a82 = 38973,    a83 = 1446,    a84 = 24249,    a85 = 115179,     a86 = 706044,     a87 = 105780,    a88 = 1154831;

    A11 =  0.1757,   A12 = -0.3643,    A13 = 0.0257,   A14 = -0.3943,    A15 = 0.4457,   A16 = -0.2143,    A17 = 0.2457,   A18 = 0.0257;
    A21 = -0.2986,   A22 = -0.2186,    A23 = 0.0914,   A24 = -0.3286,    A25 = 0.5114,   A26 = -0.1486,    A27 = 0.3114,   A28 = 0.0914;
    A31 = -0.1057,   A32 = -0.5657,    A33 = 0.1843,   A34 = -0.1557,    A35 = 0.4743,   A36 = -0.1857,    A37 = 0.2743,   A38 = 0.0543;
    A41 = -0.0457,   A42 = -0.5057,    A43 = -0.0957,  A44 = -0.1757,    A45 = 0.5343,   A46 = -0.1257,    A47 = 0.3343,   A48 = 0.1143;
    A51 = -0.0800,   A52 = -0.5400,    A53 = 0.0800,   A54 = -0.3400,    A55 = 0.3300,   A56 =  0.1700,    A57 = 0.3000,   A58 = 0.0800;
    A61 =  0.0143,   A62 = -0.4457,    A63 = 0.1743,   A64 = -0.2457,    A65 = 0.2643,   A66 = -0.2357,    A67 = 0.3943,   A68 = 0.1743;
    A71 = -0.0457,   A72 = -0.5057,    A73 = 0.1143,   A74 = -0.3057,    A75 = 0.5343,   A76 = -0.1257,    A77 = 0.1443,   A78 = 0.2243;
    A81 = -0.0143,   A82 = -0.4743,    A83 = 0.1457,   A84 = -0.2743,    A85 = 0.5657,   A86 = -0.0943,    A87 = 0.2557,   A88 = -0.0443;

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
//    ofstream fp_pimage;
//    fp_pimage.open("../data/plain_image.txt");
    for(i=0; i<height; i++){
        for(j=0; j<width; j++){
            value = pimage->pixel(j, i);     // row first;
            r[i*width+j] = qRed(value);
            g[i*width+j] = qGreen(value);
            b[i*width+j] = qBlue(value);
//            fp_pimage << r[i*width+j] << " " << g[i*width+j] << " " << b[i*width+j] << endl;
        }
    }
//    fp_pimage.close();
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
//    ofstream fp_cimage;
//    fp_cimage.open("../data/cipher_image.txt");
//    for(index=0; index<65536; index++){
//        fp_cimage << r_scramble[index] << " "
//                  << g_scramble[index] << " "
//                  << b_scramble[index] << endl;
//    }
//    fp_cimage.close();
    delete [] r;
    delete [] g;
    delete [] b;
    /*------------- pixel value encryption -------------*/
    double x1,x2,x3,x4,x5,x6,x7,x8;
    for(index=0; index<65536; index++){
        r_encrypt[index] = r_encrypt[index] ^ ((int)X1 % 256);
        g_encrypt[index] = g_encrypt[index] ^ ((int)X2 % 256);
        b_encrypt[index] = b_encrypt[index] ^ ((int)X3 % 256);
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
    /*------------------ show image -------------------*/
//    QByteArray image_byte_array = QByteArray((const char*)rgb_buffer, byte_count);
//    uchar *trans_data = (uchar*)image_byte_array.data();
//    QImage *cimage = new QImage(trans_data, width, height, QImage::Format_RGB888);
//    QMessageBox::information(this, "", "");
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
            value = cimage->pixel(j, i);     // row first;
            r_encrypt[i*width+j] = qRed(value);
            g_encrypt[i*width+j] = qGreen(value);
            b_encrypt[i*width+j] = qBlue(value);
        }
    }
    /*------------- pixel value decryption -------------*/
    double x1,x2,x3,x4,x5,x6,x7,x8;
    for(index=0; index<65536; index++){
        r_encrypt[index] = r_encrypt[index] ^ ((int)X1 % 256);
        g_encrypt[index] = g_encrypt[index] ^ ((int)X2 % 256);
        b_encrypt[index] = b_encrypt[index] ^ ((int)X3 % 256);
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
    m_label_cipher_image->setPixmap(QPixmap::fromImage(*aimage));

    QString a_path_img = QFileDialog::getSaveFileName(this,
                                            tr("Save image"),
                                            "",
                                            tr("Images (*.png *.bmp)")
                                            );
    aimage->save(a_path_img,"bmp");
    m_image_path->setText(a_path_img);
}
