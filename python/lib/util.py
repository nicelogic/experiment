
class A:
    valud = 3

def fun():
    print('fun')

def convert_dictlist_to_list(a_dict_list, key):
    res_list = []
    for dict_val in a_dict_list:
        res_list.append(dict_val[key])
    return res_list