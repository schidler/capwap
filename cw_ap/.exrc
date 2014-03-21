if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
nmap gx <Plug>NetrwBrowseX
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
map <F4> :call TitleDet()'s
let &cpo=s:cpo_save
unlet s:cpo_save
set backspace=indent,eol,start
set directory=~/tmp,/var/tmp,/tmp
set fileencodings=ucs-bom,utf-8,latin1
set guicursor=n-v-c:block,o:hor50,i-ci:hor15,r-cr:hor30,sm:block,a:blinkon0
set helplang=en
set history=50
set hlsearch
set nomodeline
set ruler
set tabstop=4
set viminfo='20,\"50
" vim: set ft=vim :
