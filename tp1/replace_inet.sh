rm client_inet
chmod +x new_client_inet
mv new_client_inet client_inet
sleep 2
./client_inet $SERV_ADDR
