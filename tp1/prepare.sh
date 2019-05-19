# !/bin/bash

echo "*** Installing libpam0g-dev, required to compile with authentication module."
sudo apt-get install libpam0g-dev -y
echo "*** Copying server_auth_mod to /etc/pam.d/"
sudo cp ./server_auth_mod /etc/pam.d/
echo "*** Installing cppcheck ***"
sudo apt-get install cppcheck -y
echo "*** Now you are ready to build and use server application."

