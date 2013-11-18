/**
 * Created by Punit Mehta
 * E-Mail : punit9462@gmail.com
 */
#include <kapplication.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <mainwindow.h>

using namespace Analitza;

int main(int argc, char *argv[])
{
    KAboutData aboutData("PlotView3DTest",
                         0,
                         ki18n("PlotView3DTest"),
                         "1.0",
                         ki18n("PlotView3DTest"),
                         KAboutData::License_LGPL_V3,
                         ki18n("Plot Demo"),
                         ki18n("PlotView3DTest"),
                         "http://www.kde.org");

    KCmdLineArgs::init(argc, argv, &aboutData);
    KCmdLineOptions options;
    options.add("all-disabled", ki18n("marks all the plots as not visible"));
    options.add("simple-rotation", ki18n("uses simple rotation"));
    KCmdLineArgs::addCmdLineOptions(options);
    KCmdLineArgs* args = KCmdLineArgs::parsedArgs();
    QApplication app ( argc , argv );
    MainWindow  *window = new MainWindow(args);
    return app.exec();
}


