str=`python -c "import sys, urllib as ul; print ul.quote_plus(\"$1\")"`
curl http://qycpu7.cse.ust.hk:8080/home/dialogue_$str/
