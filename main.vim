" Adding Git to $PATH
if $GIT_ROOT == ""
    echoerr "Environment variable GIT_ROOT is undefined."
else
    let $PATH .= ";" . $GIT_ROOT . "/bin"
endif

" Adding Cmake to $PATH
if $CMAKE_ROOT == ""
    echoerr "Environment variable CMAKE_ROOT is undefined."
else
    let $PATH .= ";" . $CMAKE_ROOT . "/bin"
endif

" Search tags asynchronously
!start vim -S "tags.vim"
