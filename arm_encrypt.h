#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    QLabel *m_image_path;
    QLabel *m_key_info;
    QLabel *m_label_plain_image;
    QLabel *m_label_cipher_image;
    QPushButton *m_button_open_image;
    QPushButton *m_button_encrypt_image;
    QPushButton *m_button_decrypt_image;

    bool is_readImg = false;
    bool is_encrypt = false;

    int table[65536];
    int a11,a12,a13,a14,a15,a16,a17,a18,
        a21,a22,a23,a24,a25,a26,a27,a28,
        a31,a32,a33,a34,a35,a36,a37,a38,
        a41,a42,a43,a44,a45,a46,a47,a48,
        a51,a52,a53,a54,a55,a56,a57,a58,
        a61,a62,a63,a64,a65,a66,a67,a68,
        a71,a72,a73,a74,a75,a76,a77,a78,
        a81,a82,a83,a84,a85,a86,a87,a88;
    double  A11,A12,A13,A14,A15,A16,A17,A18,
            A21,A22,A23,A24,A25,A26,A27,A28,
            A31,A32,A33,A34,A35,A36,A37,A38,
            A41,A42,A43,A44,A45,A46,A47,A48,
            A51,A52,A53,A54,A55,A56,A57,A58,
            A61,A62,A63,A64,A65,A66,A67,A68,
            A71,A72,A73,A74,A75,A76,A77,A78,
            A81,A82,A83,A84,A85,A86,A87,A88;
    double X1,X2,X3,X4,X5,X6,X7,X8;
    int B1,B2,B3,E1,E2,E3;

    void EncryptCatInit();

private slots:
    void openImage();
    void encryptImage();
    void decryptImage();
};

#endif // DIALOG_H
