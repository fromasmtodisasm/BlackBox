let &path.="include,"
set makeprg=./build.sh

nnoremap <F4> :make!<cr>
nnoremap <F5> :!./run.sh<CR>
map <C-n> :NERDTreeToggle<CR>
