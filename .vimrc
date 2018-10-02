filetype indent plugin on 

syntax on

set nocompatible

set number cursorline

set incsearch hlsearch

set fileencoding=utf-8 encoding=utf-8

set expandtab softtabstop=2 tabstop=2 shiftround shiftwidth=2

set nobackup

set cc=80
set showmatch
set backspace=indent,eol,start

set laststatus=2
set showmode

set autowrite

set scrolloff=5
set nowrap
set mouse=a

set foldenable
set foldmethod=manual
set foldlevelstart=0

set statusline=%F%m%r\ [EOL=%{&fileformat}]\ [TYPE=%Y]\ [ASCII=%03.3b]\ [POS=%04l,%04v]\ [%p%%]\ [LINES=%L]

if !exists('g:os')
  if has('win64') || has ('win32')
    let g:os = 'Windows'
  else
    let g:os = substitute(system('uname'), '\n', '', '')
  endif
endif

function! Build()
  let build_tab = bufwinnr("__BUILD__")
  let build_cmd = 'pushd build && ' . g:build_cmds[g:os][g:build_output] . " && popd\<CR>"
  if build_tab == -1
    tabedit __BUILD__ 
    terminal ++curwin
  else
    execute build_tab . 'wincmd w'
  endif
  call term_sendkeys(build_tab, build_cmd)
endfunction

inoremap <C-b> :call Build()<CR>
nnoremap <C-b> :call Build()<CR>