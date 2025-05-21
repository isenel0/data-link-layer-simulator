#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <fstream>
#include <vector>
#include <iostream>

class DataLinkSimulator : public QMainWindow {
private:
    QPushButton* browseButton;
    QLabel* filePathLabel;
    std::vector<uint8_t> fileData; // Okunan dosyanın içeriğini byte olarak tutacak

public:
    DataLinkSimulator(QWidget* parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Data Link Layer Simulator");
        setMinimumSize(800, 600);

        // Ana widget ve layout oluştur
        QWidget* centralWidget = new QWidget(this);
        QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
        
        // Dosya seçme butonu
        browseButton = new QPushButton("Dosya Seç (.dat)", this);
        mainLayout->addWidget(browseButton);
        
        // Dosya yolu gösterme etiketi
        filePathLabel = new QLabel("Dosya seçilmedi", this);
        mainLayout->addWidget(filePathLabel);
        
        // Ana layout'u central widget'a ata
        centralWidget->setLayout(mainLayout);
        setCentralWidget(centralWidget);
        
        // Sinyal bağlantısı
        connect(browseButton, &QPushButton::clicked, this, &DataLinkSimulator::browseDatFile);
    }

    void browseDatFile() {
        // Dosya seçme diyaloğunu aç (.dat dosyaları için filtreli)
        QString filePath = QFileDialog::getOpenFileName(
            this,
            "DAT Dosyası Seç",
            QString(),
            "Data Dosyaları (*.dat);;Tüm Dosyalar (*)"
        );
        
        if (filePath.isEmpty()) {
            return; // Kullanıcı iptal etmiş
        }
        
        // Dosya yolunu göster
        filePathLabel->setText("Seçilen dosya: " + filePath);
        
        // Dosyayı oku
        if (readDataFile(filePath.toStdString())) {
            QMessageBox::information(
                this,
                "Başarılı",
                QString("Dosya başarıyla okundu. Toplam %1 byte veri.").arg(fileData.size())
            );
            
            // Burada okunan veriyi kullanarak diğer işlemlere geçebilirsiniz
            processByteData();
        }
    }
    
    bool readDataFile(const std::string& filePath) {
        std::ifstream file(filePath, std::ios::binary);
        
        if (!file) {
            QMessageBox::critical(this, "Hata", "Dosya açılamadı: " + QString::fromStdString(filePath));
            return false;
        }
        
        // Dosyayı byte byte oku
        fileData.clear();
        char byte;
        while (file.get(byte)) {
            fileData.push_back(static_cast<uint8_t>(byte));
        }
        
        file.close();
        return true;
    }
    
    void processByteData() {
        // Burada okunan veriyi bitlere dönüştürme ve 
        // çerçevelere bölme işlemlerini yapacağız
        // (Şu an için sadece kaç byte okunduğunu yazdırıyoruz)
        std::cout << "Okunan toplam veri: " << fileData.size() << " byte" << std::endl;
        
        // İlk birkaç byte'ı göster (debug için)
        int showBytes = std::min(10, static_cast<int>(fileData.size()));
        std::cout << "İlk " << showBytes << " byte:" << std::endl;
        
        for (int i = 0; i < showBytes; i++) {
            std::cout << "Byte " << i << ": " << static_cast<int>(fileData[i]) 
                      << " (ASCII: " << static_cast<char>(fileData[i]) << ")" << std::endl;
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    DataLinkSimulator window;
    window.show();
    return app.exec();
}