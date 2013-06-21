

for dir in $@ ; do
    files=./$dir/${$(ls "$1")}
    echo $files
done