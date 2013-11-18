/**
 * Created by Punit Mehta
 * E-Mail : punit9462@gmail.com
 */
#include "mainwindow.h"
#include <QColor>
#include <QtCore/QStringList>
#include <QTreeView>
#include <QMenu>
#include <QMessageBox>

using namespace Analitza;

MainWindow::MainWindow(KCmdLineArgs* args)
{
    this->setMinimumSize(640, 480);
    m_menu = new QMenu(this);
    QSplitter *central = new QSplitter(Qt::Vertical, this);
    central->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_model = new PlotsModel(central);
    m_size2=0;
    m_treeview = new QTreeView(this);
    QLabel *label = new QLabel("Equation",this);
    m_textfield = new QLineEdit(this);
    QPushButton *plotb = new QPushButton(this);
    plotb->setText("Plot");
    m_view3d = new PlotsView3D(this);
    m_sfactory = PlotsFactory::self();
    m_view3d->setUseSimpleRotation(args->isSet("simple-rotation"));
    QWidget *widget=new QWidget(this);
    m_listwidget = new QListWidget(this);
    m_listwidget->setVisible(false);
    m_listwidget2 = new QListWidget(this);
    m_listwidget2->setVisible(false);
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(label,0,0);
    layout->addWidget(m_textfield,0,1);
    layout->addWidget(plotb,0,2);
    layout->addWidget(m_view3d,1,0,1,2);
    layout->addWidget(m_treeview,1,2);
    widget->setLayout(layout);
    setCentralWidget(widget);
    widget->connect(plotb,SIGNAL(clicked()),this,SLOT(buttonclicked()));
    m_random=0;
    m_size=0;
    setColorList();
    createActions();
    m_treeview->setModel(m_model);
    m_view3d->setModel(m_model);
    m_treeview->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
    this->show();
}
MainWindow::~MainWindow()
{
}
void MainWindow::setColorList()
{
    m_list.append(Qt::green);
    m_list.append(Qt::yellow);
    m_list.append(Qt::cyan);
    m_list.append(Qt::red);
    m_list.append(Qt::darkCyan);
    m_list.append(Qt::magenta);
    m_list.append(Qt::darkGreen);
    m_list.append(Qt::blue);
    m_list.append(Qt::white);
}
void MainWindow::buttonclicked()
{
    m_equation = m_textfield->text();
    if(m_equation=="")
    {
        QMessageBox::warning(this,tr("Error"),tr("Equation is not available to plot"),QMessageBox::Ok);
    }
    else
    {
        m_listwidget->insertItem(m_size,m_equation);
        m_model->addPlot(m_sfactory->requestPlot(Analitza::Expression(m_listwidget->item(m_size)->text()), Dim3D).create(m_list.at(m_random%9),"implicit 2"));
        m_random++;
        m_size++;
    }
}
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    m_menu->addAction(m_ChangeEquation);
    m_menu->addAction(m_DeleteEquation);
    m_menu->addAction(m_HideEquation);
    m_menu->exec(event->globalPos());
}
void MainWindow::createActions()
{
    m_ChangeEquation = new QAction("Change Plot",this);
    m_ChangeEquation->setStatusTip(tr("To change the function , you want to plot"));
    connect(m_ChangeEquation, SIGNAL(triggered()), this, SLOT(changeActionclicked()));
    m_DeleteEquation = new QAction("Delete Plot",this);
    m_DeleteEquation->setStatusTip(tr("To delete the function , you want to plot"));
    connect(m_DeleteEquation, SIGNAL(triggered()), this, SLOT(deleteActionclicked()));
    m_HideEquation = new QAction("Hide/Show Plot",this);
    m_HideEquation->setStatusTip(tr("To hide the function , you want to plot"));
    connect(m_HideEquation, SIGNAL(triggered()), this, SLOT(hideActionclicked()));
}
void MainWindow::afteritemchanged(QListWidgetItem *item)
{
    item->setFlags( item->flags() | Qt::ItemIsEditable );
    m_rowchanged=m_listwidget->row(item);
}
void MainWindow::afteritemchanged2(QListWidgetItem *item)
{
    item->setFlags( item->flags() | Qt::ItemIsEditable );
    m_rowchanged2=m_listwidget2->row(item);
}
void MainWindow::changeActionclicked()
{
    m_changeActionDialogue = new QWidget(this);
    m_changeActionDialogue->resize(200,200);
    QPoint point;
    point.setX(100);
    point.setY(300);
    m_changeActionDialogue->move(point);
    QPushButton *submit = new QPushButton("Submit",m_changeActionDialogue);
    QPushButton *close = new QPushButton("Close",m_changeActionDialogue);
    QGridLayout *lout = new QGridLayout(m_changeActionDialogue);
    lout->addWidget(m_listwidget,0,0,1,0);
    lout->addWidget(submit,1,0);
    lout->addWidget(close,1,1);
    m_changeActionDialogue->setLayout(lout);
    connect(submit,SIGNAL(clicked()),this,SLOT(changesubmitClicked()));
    connect(close,SIGNAL(clicked()),this,SLOT(changecloseClicked()));
    connect(m_listwidget, SIGNAL (itemDoubleClicked (QListWidgetItem *)), this, SLOT (afteritemchanged(QListWidgetItem *)));
    m_listwidget->setVisible(true);
    m_changeActionDialogue->setAutoFillBackground(true);
    m_changeActionDialogue->show();
}
void MainWindow::changesubmitClicked()
{
    ReimplementPlots();
}
void MainWindow::ReimplementPlots()
{
    m_model->removeRow(m_rowchanged);
    m_model->addPlot(m_sfactory->requestPlot(Analitza::Expression(m_listwidget->item(m_rowchanged)->text()), Dim3D).create(m_list.at(m_random%9),"implicit 2"));
    m_view3d->setVisible(false);
    correction();
    m_random++;
    m_changeActionDialogue->close();
}
void MainWindow::correction()
{
    for(int i=1;i<=(m_size-m_rowchanged-1);i++)
    {
        m_model->removeRow(m_rowchanged);
        QString str=m_listwidget->item(m_rowchanged+i)->text();
        m_random++;
        m_model->addPlot(m_sfactory->requestPlot(Analitza::Expression(str), Dim3D).create(m_list.at(m_random%9),"implicit 2"));
    }
    m_view3d->setVisible(true);
}
void MainWindow::changecloseClicked()
{
    m_changeActionDialogue->close();
}
void MainWindow::hideActionclicked(){
    m_listwidget->setVisible(true);
    m_listwidget2->setVisible(true);
    m_hideActionDialogue = new QWidget(this);
    m_hideActionDialogue->resize(300,300);
    QPoint point;
    point.setX(20);
    point.setY(10);
    m_hideActionDialogue->move(point);
    QPushButton *hideb = new QPushButton("Hide",m_hideActionDialogue);
    QPushButton *close = new QPushButton("Close",m_hideActionDialogue);
    QPushButton *showb = new QPushButton("Show",m_hideActionDialogue);
    QGridLayout *lout = new QGridLayout;
    lout->addWidget(m_listwidget,0,0);
    lout->addWidget(m_listwidget2,0,1);
    lout->addWidget(hideb,1,0);
    lout->addWidget(showb,1,1);
    lout->addWidget(close,2,0,2,0);
    m_hideActionDialogue->setLayout(lout);
    connect(hideb,SIGNAL(clicked()),this,SLOT(hideClicked()));
    connect(close,SIGNAL(clicked()),this,SLOT(hidecloseClicked()));
    connect(showb,SIGNAL(clicked()),this,SLOT(showClicked()));
    connect(m_listwidget, SIGNAL (itemClicked (QListWidgetItem *)), this, SLOT (afteritemchanged(QListWidgetItem *)));
    connect(m_listwidget2, SIGNAL (itemClicked (QListWidgetItem *)), this, SLOT (afteritemchanged2(QListWidgetItem *)));
    m_hideActionDialogue->setAutoFillBackground(true);
    m_hideActionDialogue->show();
}
void MainWindow::hideClicked()
{
    QString str = m_listwidget->takeItem(m_rowchanged)->text();
    m_listwidget2->insertItem(m_size2,str);
    m_size2++;
    if(m_size>0)
    {
        m_size--;
    }
    m_model->removeRow(m_rowchanged);
}
void MainWindow::showClicked()
{
    QString str = m_listwidget2->takeItem(m_rowchanged2)->text();
    m_size2--;
    m_model->addPlot(m_sfactory->requestPlot(Analitza::Expression(str), Dim3D).create(m_list.at(m_random%9),"implicit 2"));
    m_random++;
    m_listwidget->insertItem(m_size,str);
    m_size++;
}
void MainWindow::hidecloseClicked()
{
    m_hideActionDialogue->close();
}
void MainWindow::deleteActionclicked()
{
    m_listwidget->setVisible(true);
    m_deleteActionDialogue = new QWidget(this);
    m_deleteActionDialogue->resize(200,200);
    QPoint point;
    point.setX(400);
    point.setY(200);
    m_deleteActionDialogue->move(point);
    QPushButton *submit = new QPushButton("Submit",m_deleteActionDialogue);
    QPushButton *close = new QPushButton("Close",m_deleteActionDialogue);
    QGridLayout *lout = new QGridLayout;
    lout->addWidget(m_listwidget,0,0,1,0);
    lout->addWidget(submit,1,0);
    lout->addWidget(close,1,1);
    m_deleteActionDialogue->setLayout(lout);
    connect(submit,SIGNAL(clicked()),this,SLOT(deletesubmitClicked()));
    connect(close,SIGNAL(clicked()),this,SLOT(deletecloseClicked()));
    connect(m_listwidget, SIGNAL (itemClicked (QListWidgetItem *)), this, SLOT (afteritemchanged(QListWidgetItem *)));
    m_deleteActionDialogue->setAutoFillBackground(true);
    m_deleteActionDialogue->show();
}
void MainWindow::deletesubmitClicked()
{
    m_model->removeRow(m_rowchanged);
    m_listwidget->takeItem(m_rowchanged);
    if(m_size>0)
    {
        m_size--;
    }
    m_deleteActionDialogue->close();
}
void MainWindow::deletecloseClicked()
{
    m_deleteActionDialogue->close();
}
