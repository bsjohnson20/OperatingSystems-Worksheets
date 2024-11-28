# Runs a tmux shell with 3 windows, split right and left, with a session at bottom

# Start a new session with our name
tmux new-session -d -s "tmux-session"

# Split the session into 3 windows
tmux split-window -h
tmux split-window -v

# echo all windows 
tmux list-windows

TEL_SER=4343
TEL_QEM=4344

# run command in top left window
tmux send-keys -t "tmux-session:0.0" "make logcpu tel_ser=${TEL_SER} tel_qem=${TEL_QEM}" C-m


# Wait for qemu telnet to init
sleep 0.2

# bottom
tmux send-keys -t "tmux-session:0.2" "while [ 1 == 1 ]; do telnet localhost ${TEL_QEM}; sleep 5; done" C-m

# right
tmux send-keys -t "tmux-session:0.1" "while [ 1==1 ]; do telnet localhost ${TEL_SER}; sleep 0.1; done" C-m

# Attach to the session
tmux attach-session -t "tmux-session"
