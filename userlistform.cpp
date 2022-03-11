#include "userlistform.h"
#include "ui_userlistform.h"

UserListForm::UserListForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserListForm)
{
    ui->setupUi(this);
    QPalette label_palette;
    label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
    setPalette(label_palette);
    ui->listWidget->setStyleSheet("background-color:transparent");

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(userListTimerCount()));
    timer->start(1000);//1m的定时器

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->label_name->setPalette(pe);
    ui->label_day->setPalette(pe);
    ui->label_time->setPalette(pe);
    ui->label_title->setPalette(pe);
    ui->label_tool->setPalette(pe);
    ui->label_id->setPalette(pe);

    QFont name_ft;
    name_ft.setPointSize(28);
    ui->label_title->setFont(name_ft);

    QFont id_ft;
    id_ft.setPointSize(18);
    ui->label_id->setFont(id_ft);
    ui->label_day->setFont(id_ft);
    ui->label_time->setFont(id_ft);
    ui->label_name->setFont(id_ft);
    ui->label_tool->setFont(id_ft);

    //query users
    dbOperator = new DbOperator("plam_data");

    initData();
    ui->pushButton_delete->setFont(QFont("宋体", 18));
    ui->pushButton_delete->setStyleSheet("QPushButton{background-color:rgba(0,0,0,100%);width:100px;height:50px;\
                                            color: white;   border-radius: 10px;  border: 2px groove gray; border-style: outset;}" // 按键本色
        "QPushButton:hover{background-color:white; color: black;}"  // 鼠标停放时的色彩
        "QPushButton:pressed{background-color:rgb(85, 170, 255); border-style: inset; }");   // 鼠标按下的色彩

}

UserListForm::~UserListForm()
{
    delete ui;
}

void UserListForm::userListTimerCount(void)
{
        QLocale locale = QLocale::Chinese;//指定中文显示
        QString strTime = locale.toString(QTime::currentTime(),QString("h:m"));
        ui->label_time->setText(strTime);
        QString strDate = locale.toString(QDate::currentDate(),QString("yyyy/M/d"));
        ui->label_day->setText (strDate);
}

void UserListForm::on_pushButton_delete_clicked()
{
    QList<QString> idList;
    for (int j = 0; j < ui->listWidget->count(); j++)
    {
        QListWidgetItem * item = ui->listWidget->item(j);
        QWidget* pwig = ui->listWidget->itemWidget(item);

        QCheckBox *checkbox = pwig->findChild<QCheckBox *>("");
        if(checkbox->isChecked()){

            QList<QLabel*> labelList = pwig->findChildren<QLabel*>();
            foreach(QLabel *label, labelList)
            {
                if(QString::fromLocal8Bit("id")==label->objectName()){
                    idList.append(label->text());
                }
            }
        }
    }
//    qDebug()<<idList.size()<<endl;
    dbOperator->deleteUsers(idList);
    ui->listWidget->clear();
    initData();
}

void UserListForm::initData(){
    QList<User> userList = dbOperator->queryAll();
    for(int i=0; i<userList.size(); ++i){
        QListWidgetItem *item = new QListWidgetItem(ui->listWidget);
        item->setSizeHint(QSize(600, 60));
        item->setBackground(Qt::black);
        item->setForeground(Qt::white);
        QWidget *w = new QWidget(ui->listWidget);
        QHBoxLayout *layout=new QHBoxLayout(w);


        QCheckBox *checkBox=new QCheckBox(w);

        checkBox->setStyleSheet(QString("QCheckBox::indicator {width: 20px; height: 20px; } "
                                        "QCheckBox::indicator:checked{image:url(:/new/res/images/checked.svg);}"
                                        "QCheckBox::indicator:unchecked{image:url(:/new/res/images/unchecked.svg);}"));


        QFont item_ft;
        item_ft.setPointSize(14);
        QLabel *idLabel = new QLabel(w);
        idLabel->setText(userList[i].id);
        idLabel->setStyleSheet("color:white;");
        idLabel->setFont(item_ft);
        idLabel->setObjectName("id");

        QLabel *nameLabel = new QLabel(w);
        nameLabel->setText(userList[i].name);
        nameLabel->setStyleSheet("color:white;");
        nameLabel->setFont(item_ft);
        nameLabel->setObjectName("name");


        QPushButton *pushButton=new QPushButton(w);
        pushButton->setText("");
        QIcon myicon; //新建QIcon对象
        myicon.addFile(tr(":/new/res/images/edit.svg")); //让QIcon对象指向想要的图标
        pushButton->setIcon(myicon);
        pushButton->setIconSize(QSize(20,20));//重置图标大小
        pushButton->setFlat(true);
        pushButton->setDefault(false);
        layout->addWidget(checkBox);
        layout->addWidget(idLabel);
        layout->addWidget(nameLabel);
        layout->addWidget(pushButton);

        layout->setStretchFactor(checkBox, 2);
        layout->setStretchFactor(idLabel, 5);
        layout->setStretchFactor(nameLabel, 5);
        layout->setStretchFactor(pushButton, 5);

        w->setLayout(layout);
        w->show();
        ui->listWidget->setItemWidget(item,w);
        ui->listWidget->show();
    }
}

void UserListForm::on_pushButton_clicked()
{
    emit backWorking();
}

void UserListForm::on_pushButton_search_clicked()
{
    emit toSearch();
}
