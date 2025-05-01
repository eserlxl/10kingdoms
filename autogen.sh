set -e

echo "Running gettextize..."
gettextize --force --copy --no-changelog

echo "Running libtoolize..."
libtoolize

echo "Running aclocal..."
aclocal

echo "Running autoheader..."
autoheader

echo "Running automake..."
automake --add-missing --foreign

echo "Running autoconf..."
autoconf

echo "Done. Now run ./configure"
