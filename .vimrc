call plug#begin('~/.vim/plugged')

call plug#end()
let &path.="include/public,include/private,"
set makeprg=./build.sh

nnoremap <F4> :make!<cr>
nnoremap <F5> :!./run.sh<CR>
map <C-n> :NERDTreeToggle<CR>
