echo .*
echo *c
ls && (echo aa | cat)
export V="aa   xxx"
echo aa > $V | pwd
echo $?
pwd | echo aa > $V
echo $?