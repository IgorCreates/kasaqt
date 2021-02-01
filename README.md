# README #
Hi, welcome to KasaQT opensource point of sale project. KasaQT has been created by Igor & Ante for Croatian Bars and Restaurants, but now has been pushed on git for everyone :). 
Feel free to take it, improve it, make it better and enjoy (and don't forget to make git push) :) 

Igor


* Version os_2.0

### How do I get set up? ###

* Summary of set up
  
  clone kasaqt make and enyoy :)
  clone kasabof make and enyoy :)


* for p3.py install all dependencies (based on https://github.com/kodmasin/fiskpy)
  pip install -r python_req.txt 

* Configuration
* Database configuration
  - add this into your my.cnf [mysqld] section
	character-set-server = utf8
	max_sp_recursion_depth = 5
	lower_case_table_names = 1
	sql_mode=''


You must have 2 databases 
  1. local 
      - local database for kasaqt, where is stored all options for POS,printing preferences,
  and locally preparation for make sales.
  2. kasa 
      - local or remote database where is stored all else
in folder SQL you is local.sql and kasa.sql prepared for create databases
   #mysql -u root -p -e 'create database local default character set utf8;'
   #mysql -u root -p -e 'create database kasa default character set utf8;'
   #mysql -u root -p local < local.sql
   #mysql -u root -p kasa < kasa.sql

* How to run tests
* Deployment instructions

### Contribution guidelines ###

* Plz commit into origin head:develop and after this create pull request to master

### Who do I talk to? ###

* Repo owner or admin
* Other community or team contact
