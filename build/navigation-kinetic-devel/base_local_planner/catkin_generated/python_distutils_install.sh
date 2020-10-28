#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/green/nav_tractor/src/navigation-kinetic-devel/base_local_planner"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/green/nav_tractor/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/green/nav_tractor/install/lib/python2.7/dist-packages:/home/green/nav_tractor/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/green/nav_tractor/build" \
    "/usr/bin/python2" \
    "/home/green/nav_tractor/src/navigation-kinetic-devel/base_local_planner/setup.py" \
    build --build-base "/home/green/nav_tractor/build/navigation-kinetic-devel/base_local_planner" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/green/nav_tractor/install" --install-scripts="/home/green/nav_tractor/install/bin"
