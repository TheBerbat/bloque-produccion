QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/algorithm/BaseMRP.cpp \
    src/algorithm/Batch2Batch.cpp \
    src/algorithm/Batch2BatchWithMin.cpp \
    src/algorithm/EOQ.cpp \
    src/algorithm/FixedPeriod.cpp \
    src/algorithm/FixedPeriodOptimal.cpp \
    src/algorithm/MinimumUnitaryCost.cpp \
    src/algorithm/MinimumTotalCost.cpp \
    src/algorithm/SilverMeal.cpp \
    src/insertdatadialog.cpp \
    src/selectdatabasedialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/finaltabledialog.cpp \
    src/aboutdialog.cpp \

HEADERS += \
    includes/mainwindow.h \
    includes/selectdatabasedialog.h \
    includes/insertdatadialog.h \
    includes/algorithm/BaseMRP.hpp \
    includes/algorithm/Batch2Batch.hpp \
    includes/algorithm/Batch2BatchWithMin.hpp \
    includes/algorithm/EOQ.hpp \
    includes/algorithm/FixedPeriod.hpp \
    includes/algorithm/FixedPeriodOptimal.hpp \
    includes/algorithm/MinimumUnitaryCost.hpp \
    includes/algorithm/MinimumTotalCost.hpp \
    includes/algorithm/SilverMeal.hpp \
    includes/finaltabledialog.h \
    includes/aboutdialog.h \

FORMS += \
    src/insertdatadialog.ui \
    src/selectdatabasedialog.ui \
    src/mainwindow.ui \
    src/finaltabledialog.ui \
    src/aboutdialog.ui \

INCLUDEPATH += includes

RC_ICONS = uclm.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
