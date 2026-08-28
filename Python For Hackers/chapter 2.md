
## int

```python

var_test = 1
print('var_test: ',var_test,type(var_test))

var_test = 1.0
print('var_test: ',var_test,type(var_test))

var_test = 2
print('var_test: ',var_test,type(var_test))

var_test = 'test'
print('var_test: ',var_test,type(var_test))
print('-'*20)
##############################################################
var_num = 10
print(var_num,type(var_num))

var_num = int('10')
print(var_num,type(var_num))

var_num = int('10101',2)
print(var_num,type(var_num))

var_num = 0x41
print(var_num,type(var_num))

var_num = 0b10101
print(var_num,type(var_num))

var_num = hex(15)
print('HEX: ',var_num,type(var_num),int(var_num,16))

var_num = bin(15)
print('BIN: ',var_num,type(var_num),int(var_num,2))

################################################################
print('-'*20)
num_1 = 10
num_2 = 4

print(num_2,'-',num_1,'=',num_2 - num_1)
print(num_2,'+',num_1,'=',num_2 + num_1)
print(num_2,'*',num_1,'=',num_2 * num_1)
print(num_2,'/',num_1,'=',num_2 / num_1)
print(num_2,'**',num_1,'=',num_2 ** num_1)
print(num_2,'%',num_1,'=',num_2 % num_1)

################################################################
print('-'*20)

num_1 = int(input("number 1:"))		#raw_input()
num_2 = int(input("number 2:"))

print(num_1,type(num_1))
print(num_2,type(num_2))

print(num_2,'-',num_1,'=',num_2 - num_1)
print(num_2,'+',num_1,'=',num_2 + num_1)
print(num_2,'*',num_1,'=',num_2 * num_1)
print(num_2,'/',num_1,'=',num_2 / num_1)
print(num_2,'**',num_1,'=',num_2 ** num_1)
print(num_2,'%',num_1,'=',num_2 % num_1)

```

یکی از قابلیت های تابع int اینه که میتونه یه عدد در مبنا هم بگیره همونطور که میبینید عدد 10101 رو من در مبنای باینری که میشه 2 بهش دادم

یه نکته یی که تابع input داره اینه که دیتا هایی که ما بهش پاس میدیم یه دیتا int هست اما اگر بیایم type رو بگیریم str بر میگردونه و در انجام عملیات اگر بیایم str رو با int  یه عملیات ریاضی انجام بدیم به مشکل میخوریم مگر اینکه cast کنیم اون دیتارو به Int یا موقع فراخوانی این تابع مستقیم int رو صدا کنیم 


## String

```python
var_str = 'RavinAcademy'
print(var_str,type(var_str))

var_str = "RavinAcademy"
print(var_str,type(var_str))

var_str = '"RavinAcademy"'
print(var_str,type(var_str))

var_str = '\'RavinAcademy\''
print(var_str,type(var_str))

# \b \t \r \n \' \"
var_str = "Ravin\t\tAcademy"
print(var_str,type(var_str))

var_str = "Ravin\n\nAcademy"
print(var_str,type(var_str))

var_str = '''RavinAcademy
Ravin
Academy
'''
print(var_str,type(var_str))

print(var_str,type(var_str))
################################################
print('-'*20)
var_str_1 = "Ravin"
var_str_2 = "Academy"
print(var_str_1,'+',var_str_2,'=',var_str_1 + var_str_2)
#################################################
print('-'*20)
print('_RavinAcademy_'*5)
#################################################
print('-'*20)
var_str_1 = str(10)
var_str_2 = str(20.1)
print(var_str_1,type(var_str_1))
print(var_str_2,type(var_str_2))
#################################################
print('-'*20)
print('Indexing: ')
var_str = '<a href="https://www.python.org"> Python Web Site </a>'
print(0,[0var_str])
print(1,var_str[1])
print(2,var_str[2])
print(-1,var_str[-1])
print(-2,var_str[-2])

print('Start',0,var_str[0])
print('End',len(var_str)-1,var_str[len(var_str)-1])

print('Start',-1*len(var_str),var_str[-1*len(var_str)])
print('End',-1,var_str[-1])
#################################################
print('-'*20)
print('Slicing: ')

var_str_1 = '<a href="https://www.python.org"> Python Web Site </a>'
var_str_2 = var_str_1[9:31]
print(var_str_2)
var_str_2 = var_str_1[9:]
print(var_str_2)
#################################################
print('-'*20)
var_str = " Ravin Academy "
#print(dir(var_str))
print(var_str)
print('find','av',var_str.find('av'))
print('find','avc',var_str.find('avc'))

print('rfind','a',var_str.rfind('a')) #reverse find

print('find','a',var_str.find('a',4))


print('index','a',var_str.index('a'))
print('rindex','a',var_str.rindex('a'))

#print('index','ab',var_str.index('ab'))

print('replace:',var_str.replace('a','bbbb'))
print('split:',var_str.split('a'))
print('split:',var_str.split())
print('none strip:',var_str)
print('strip,rstrip,lstrip:',var_str.strip())
print('strip:'+var_str.strip())
#################################################
print('-'*20)
var_chr_1 = 'a'
print(var_chr_1,type(var_chr_1))
var_chr_1 = ord(var_chr_1)
print(var_chr_1,type(var_chr_1))
var_chr_1 = chr(97)
print(var_chr_1,type(var_chr_1))
```


- \r -----> return 
- \b -----> back space
- \n ------> newline
- \t -------> tab


---

بریم باهم دیگه کد ASCII کاراکتر مون رو بگیریم  در زبان python با استفاه از تابع ord میتونیم بیایم کد ASCII هر کاراکتر رو بگیریم و با استفاده از chr میتونیم همین فرایند رو معکوس کنیم 



## list

```python

var_list_1 = [1,2,'a','RavinAcademy',10.1,[1,2]]
print(var_list_1,type(var_list_1))
#####################################################
print('-'*20)
print(0,var_list_1[0])
print(2,var_list_1[2])
print(3,var_list_1[3][0:5])
print(-1,var_list_1[-1])
print(len(var_list_1)-1,var_list_1[len(var_list_1)-1])

print(-1,1,var_list_1[-1][1])
#####################################################
print('-'*20)
var_list_1 = list('RavinAcademy')
print(var_list_1,type(var_list_1))
var_list_1 = list()
print(var_list_1,type(var_list_1))
var_list_1 = []
print(var_list_1,type(var_list_1))
#####################################################
print('-'*20)
var_list_1 = []
#print(dir(var_list_1))
var_list_1.append(1)
var_list_1.append('a')
var_list_1.append(2*3)
var_list_1.append('Ravin')
var_list_1.append([1,'ravin'])
print('append: ',var_list_1)

var_list_1.insert(1,'Python')
print('insert: ',var_list_1)

out = var_list_1.pop(1)
print('pop(1): ',out,var_list_1)

var_list_1.remove('Ravin')
print('remove(\'Ravin\')',var_list_1)

var_list_1 = [1,2] + ['ravin','test']
print(var_list_1)
var_list_1 = [41]*10
print(var_list_1)

print('SizeOf',var_list_1.__sizeof__())
print('LEN',len(var_list_1))
```

متود __siezof__ میاد سایز اون لیست رو داخل مموری بهمون نشون میده 



ما جدا از لیست یک نوع ساختار داده دیگری هم داریم تحت عنوان tuple که تنها تفاوتی که با لیست داره اینه که تغییر ناپذیره و هر تغییری در سختار داده تاپل موجب اکسپشن میشه 


## tuple

```python
var_tuple_1 = (1,2,'Ravin','a',[1,2],(1,2))
print(var_tuple_1,type(var_tuple_1))

#print(dir(var_tuple_1))
print(var_tuple_1.__sizeof__())
print(var_tuple_1[1])
print(var_tuple_1[-1])
print(var_tuple_1[-1][1])

var_tuple_1 = tuple()
var_tuple_1 = ()
var_tuple_1 = tuple('RavinAcademy')
```




## Dict

```python
var_dict_1 = {'KEY':'Value','ravin':10}
print(var_dict_1,type(var_dict_1))

print(var_dict_1['KEY'])
print(var_dict_1['ravin'])


var_dict_1 = dict([('a',1),('b',2),('c',3)])
print(var_dict_1,type(var_dict_1))

var_dict_1 = {}
print(var_dict_1,type(var_dict_1))
var_dict_1 = dict()
print(var_dict_1,type(var_dict_1))

#print(dir(var_dict_1))
var_dict_1 = dict([('a',1),('b',2),('c',3)])
print('get(\'a\')',var_dict_1.get('a'))
print('get(\'ee\')',var_dict_1.get('ee'))

print(var_dict_1)
var_dict_1.update({'Cookie':'Mozila 5.0'})
print('update',var_dict_1)

del var_dict_1['Cookie']
print('del',var_dict_1)
```


## Condenishal

```python
var_b = True
print(var_b,type(var_b))
var_b = False
print(var_b,type(var_b))

var_b = bool(1)
print(1,var_b,type(var_b))

var_b = bool(0)
print(0,var_b,type(var_b))

var_b = bool('a')
print('a',var_b,type(var_b))

var_b = bool('')
print('',var_b,type(var_b))

var_b = bool([1])
print([1],var_b,type(var_b))

var_b = bool([])
print([],var_b,type(var_b))

var_b = bool((1,2))
print((1,2),var_b,type(var_b))

var_b = bool(())
print((),var_b,type(var_b))

var_b = bool({'a':1})
print({'a':1},var_b,type(var_b))

var_b = bool({})
print({},var_b,type(var_b))

################################################################
print('-'*20)
var_list_1 = [1,2,3,4,5,6,'Ravin']
print(var_list_1,1 in var_list_1)
print(var_list_1,'a' in var_list_1)

var_str_1 = 'ravinacademy'
print(var_str_1,'a' in var_str_1)
print(var_str_1,'z' in var_str_1)
################################################################
print('-'*20)
var_list_1 = [1,2,3,4,5,6,'Ravin']
print(var_list_1,1 not in var_list_1)
print(var_list_1,'a' not in var_list_1)

var_str_1 = 'ravinacademy'
print(var_str_1,'a' not in var_str_1)
print(var_str_1,'z' not in var_str_1)
#################################################################
print('-'*20)
x = 10
y = 20
print('x = ' + str(x) + ' y = ' + str(y))
print('x == y', x == y)
print('x != y',x != y)
print('x > y', x > y)
print('x < y',x < y)
print('x >= y', x >= y)
print('x <= y',x <= y)


```


- عدد 0 **FALSE** در نظر گرفته میشه 
- عدد 1 **TRUE** در نظر گرفته میشه 

