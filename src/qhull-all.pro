# -------------------------------------------------
# qhull-all.pro -- Qt project to build executables and static libraries
#
# To build with Qt on mingw
#   Download Qt SDK, install Perl
#   /c/qt/2010.05/qt> ./configure -static -platform win32-g++ -fast -no-qt3support
#
# To build DevStudio sln and proj files (Qhull ships with cmake derived files)
# qmake is in Qt's bin directory
# mkdir -p build && cd build && qmake -tp vc -r ../src/qhull-all.pro
# Additional Library Directories -- C:\qt\Qt5.2.0\5.2.0\msvc2012_64\lib
# libqhullcpp and libqhullstatic refered to $(QTDIR) but apparently didn't retrieve (should be %QTDIR%?)
# libqhull_r also needs ..\..\lib
# Need to change build/x64/Debug/*.lib to lib/ (or copy libs by hand, each time)
# Additional Build Dependencies
# See README.txt -- Need to add Build Dependencies, disable rtti, rename targets to qhull.dll, qhull6_p.dll and qhull6_pd.dll
# -------------------------------------------------

TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += libqhull_r      #shared library with reentrant code
SUBDIRS += libqhullstatic  #static library
SUBDIRS += libqhullstatic_r #static library with reentrant code
SUBDIRS += libqhullcpp     #static library for C++ interface with libqhullstatic_r

SUBDIRS += qvoronoi
