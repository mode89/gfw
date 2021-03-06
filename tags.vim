let cwd = getcwd()

" Search for tags in the following directories
let tagPaths = [
  \ $BOOST_ROOT . "/boost",
  \ $BOOST_ROOT . "/libs",
  \ cwd ]

" Find files with tags
let fileList = []
for path in tagPaths
    execute "cd " . path
    let files = system( "dir /s /b *.cpp *.c *.h *.hpp" )
    let fileList += split( files )
    execute "cd " . cwd
endfor

" Remove unnecessary tags generated by Boost
call filter( fileList, 'v:val !~ "boost\\\\typeof"' )
call filter( fileList, 'v:val !~ "\\\\doc\\\\"' )
call filter( fileList, 'v:val !~ "\\\\example\\\\"' )
call filter( fileList, 'v:val !~ "\\\\examples\\\\"' )
call filter( fileList, 'v:val !~ "\\\\samples\\\\"' )
call filter( fileList, 'v:val !~ "\\\\libs\\\\.\\+\\\\test\\\\"' )

" Save list of the files
call writefile( fileList, "ctags.files" )

" Run ctags
call system( "ctags --file-scope=no -L ctags.files" )

execute "q"
