" ===================vundle config==================
set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

" The following are examples of different formats supported.
" Keep Plugin commands between vundle#begin/end.
" plugin on GitHub repo
" Plugin 'tpope/vim-fugitive'
" plugin from http://vim-scripts.org/vim/scripts.html
" Plugin 'L9'
" Plugin 'taglist.vim'
" Plugin 'majutsushi/tagbar'
" Plugin 'jlanzarotta/bufexplorer'
" Plugin 'winmanager'
" Plugin 'genutils'
" Plugin 'lookupfile'
" Plugin 'cscope.vim'
Plugin 'Valloric/YouCompleteMe'
Plugin 'Valloric/ListToggle'
Plugin 'rdnetto/YCM-Generator'
Plugin 'thaerkh/vim-workspace'
" Plugin 'tell-k/vim-autopep8'
" Plugin 'Yggdroot/indentLine'
Plugin 'jiangmiao/auto-pairs'
Plugin 'scrooloose/nerdcommenter'
" Plugin 'w0rp/ale'
" Plugin 'junegunn/fzf.vim'
" Plugin 'peaksea'
" Plugin 'altercation/vim-colors-solarized'
" Plugin 'wesgibbs/vim-irblack'
" Plugin 'vim-scripts/mayansmoke'
Plugin 'ctrlpvim/ctrlp.vim'
" Plugin 'wting/gitsessions.vim'
" Plugin 'xolox/vim-misc'
" Plugin 'xolox/vim-session'
" Plugin 'chazy/cscope_maps'
" Git plugin not hosted on GitHub
" Plugin 'git://git.wincent.com/command-t.git'

" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" Put your non-Plugin stuff after this line





"===================tutorial config=====================
"Set mapleader
let mapleader = ","
set encoding=utf-8
"ycm
"let g:ycm_path_to_python_interpreter = '/home/zhicheng/anaconda3/envs/caffe/bin/python'
nnoremap <F12> :YcmCompleter GoToDefinitionElseDeclaration<CR>
nnoremap <F5> :YcmForceCompileAndDiagnostics<CR>
map <F3> :YcmCompleter FixIt<CR>

let g:ycm_confirm_extra_conf = 0

"let g:ycm_server_keep_logfiles = 1
"let g:ycm_server_log_level = 'debug'
nnoremap <leader>r :lnext<CR>

" vim workspace 
nnoremap <leader>s :ToggleWorkspace<CR>
" nerd commenter 
map <F4> <leader>ci <CR>

"按F10运行python"
map <F7> :w<CR> :call RunPythonMine()<CR>
function RunPythonMine()
  let mp = &makeprg
  let ef = &errorformat
  let exeFile = expand("%:t")
  setlocal makeprg=python\ -u
  set efm=%C\ %.%#,%A\ \ File\ \"%f\"\\,\ line\ %l%.%#,%Z%[%^\ ]%\\@=%m
  silent make %
  copen
  let &makeprg = mp
  let &errorformat = ef
endfunction
"====================my config=========================
" my own settings
set number
set mouse=a
set tabstop=2
set shiftwidth=2
set expandtab
set backspace=indent,eol,start

set cursorline  
hi CursorLine   cterm=NONE ctermbg=grey ctermfg=white guibg=darkred guifg=white  

" colorscheme ir_black
" clipboard share via ssh
vmap "+y :!xclip -f -sel clip
map "+p :r!xclip -o -sel clip
