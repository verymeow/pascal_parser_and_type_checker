#include<bits/stdc++.h>
#define tr(c,i) for(typeof((c).begin() i = (c).begin(); i != (c).end(); i++) 
using namespace std;

string var_decs[300];
int line;
void parse(string a, int mode);
vector<string> v;
map<string, string> variables;
map<string, string> variables2;
map<string, string> types;
map<string, map<string, string> > structures;
map<string, map<string, string> > func;
map<string, vector<string> > nequi;
vector<vector<string> > intnequi;
map<string, pair<pair<int,int>, string>> arr;
map<string, string> funcret;
string sname;
int matrix[1000][1000];
int tmatrix[1000][1000];
int comp(string a, string b);
vector<string> mat;
int matdim;
map<string, vector<string> > str2;
map<string, vector<string> > fun2;
int ne(string a, string b);
int ine(string a, string b);
void se();
int chse(string a, string b);
//vector<string> allnames;

int main(){
	ifstream fin("inputfile.txt");
	int i = 0;
	while(getline(fin, var_decs[i])){
		i++;
	}
	for (line = 0; line < i; line++){
		parse(var_decs[line], 0);  // zero for declarations outside structures
	}
	int numstrvars = 0, numfunvars = 0;
	tr(structures, it){
		tr(it->second, it2){
			numstrvars++;
		}
	}
	tr(func, it){
		tr(it->second, it2){
			numfunvars++;
		}
	}
	tr(variables2, it){
		mat.push_back(it->first);
	}
	tr(func, it){
		mat.push_back(it->first);
	}
	matdim = mat.size();
	se();
	for(int h=0;h<matdim;h++){
		for(int s=0;s<matdim;s++){
			if(matrix[h][s]==0)
				matrix[h][s]=1;
			else
				matrix[h][s] = 0;
		}
	}
	string a, b,c,d;
	while(1){
		cout<<"Enter two variables: "<<endl;
		cin>>a>>b;
		c=a; d=b;
		int ans; 
		int m,n;
		for(m =0;m<mat.size();m++){
			if(mat[m]==c)
				break;
		}
		for(n =0;n<mat.size();n++){
			if(mat[n]==d)
				break;
		}
		cout<<"Indices in matrix are: "<<m<<" and "<<n<<" respectively."<<endl;
		if(variables2.find(a)!=variables2.end()){
			a = variables2[a];
		}
		if(variables2.find(b)!=variables2.end()){
			b = variables2[b];
		}
		while(types.find(a)!=types.end()){
			a = types[a];
		}
		while(types.find(b)!=types.end()){
			b = types[b];
		}
		if(structures.find(a)!=structures.end() && structures.find(b)!=structures.end()){
			ans = chse(c,d);
		}
		else if(func.find(a)!=func.end() && func.find(b)!=func.end()){
			ans = chse(c,d);
		}
		else if(a.find('^')!= string::npos && b.find('^')!=string::npos){
			ans = ine(c,d);
		}
		else if(arr.find(a)!=arr.end() && arr.find(b)!=arr.end()){
			ans = chse(c,d);
		}
		else{
			ans = ne(c,d);
		}
		if(ans == 1|| ans == 2 || ans == 3){
			cout<<"True, ";
		}
		if(ans == 10|| ans == 20 || ans == 30){
			cout<<"False, ";
		}
		cout<<c<<" and "<<d<<" are ";
		if(ans == 10|| ans == 20 || ans == 30){
			cout<<"not ";
		}
		if(ans == 1|| ans==10){
			cout<<"Name Equivalent."<<endl;	
		}
		if(ans == 2|| ans==20){
			cout<<"Internal Name Equivalent."<<endl;	
		}
		if(ans == 3|| ans==30){
			cout<<"Structurally Equivalent."<<endl;	
		}
	}
	fin.close();
	return 0;
}

void parse(string a, int mode){
	if(a.find(':') != string::npos && a.find("function") == string::npos && a.find(" of ") == string::npos){// GENERIC VAR DECS
		v.clear();
		string temp;
		int p,j;
		for(p = 0; p<a.size();p++){
			temp = "";
			for(j = p;j<a.size();j++){
				if(a[j]!=' ' && a[j] != ':' && a[j] != ',' && a[j] != '\t' && a[j] != ';' && a[j] != '\n'){
					temp.push_back(a[j]);
				}
				else
					break;
			}
			p = j;
			if(temp!="" && temp != "\n" && temp != "\t")
				v.push_back(temp);
		}
		temp = v[v.size()-1];
		vector<string> vec;
		vec.clear();
		for(int pp = 0; pp<v.size()-1; pp++){
			variables[v[pp]] = temp;
			//cout<<mode<<endl;
			//cout<<v[pp]<<endl;
			if(mode == 0){
				variables2[v[pp]] = temp;
				//	cout<<v[pp]<<endl;
			}
			else if(mode == 1)
				continue;
			else
				continue;
			nequi[temp].push_back(v[pp]);
			vec.push_back(v[pp]);
		}	
		intnequi.push_back(vec);
		if(mode == 1){
			for(int d = 0; d<v.size()-1;d++){
				//cout<<sname<<endl;
				//cout<<v[d]<<endl;
				structures[sname][v[d]] = temp;
				str2[sname].push_back(temp);
			}
		}
		if(mode == 2){
			for(int d = 0; d<v.size()-1;d++){
				//cout<<vec[d]<<endl;
				func[sname][v[d]] = temp;
				fun2[sname].push_back(temp);
			}
		}
	}	
	else if(a.find('=') != string::npos && a.find("record") != string::npos && a.find("array [") == string::npos){//STRUCTS
		sname = "";
		for(int b = 0; b<a.size();b++){
			if(a[b]!=' ' && a[b] != ':' && a[b] != ',' && a[b] != '\t' && a[b] != ';' && a[b] != '\n'){
				sname.push_back(a[b]);
			}
			else if(a[b]!='=')
				break;	
			else
				continue;
		}
		line++;
		while(var_decs[line]!="end;"){
			//cout<<sname<<endl;	
			parse(var_decs[line], 1); // For declaration inside a structure	
			line++;
		}
	}
	else if(a.find('=') != string::npos && a.find("array [") == string::npos){//TYPEDEFS
		v.clear();
		string temp;
		int p,j;
		for(p = 0; p<a.size();p++){
			temp = "";
			for(j = p;j<a.size();j++){
				if(a[j]!=' ' && a[j] != '=' && a[j] != ',' && a[j] != '\t' && a[j] != ';' && a[j] != '\n'){
					temp.push_back(a[j]);
				}
				else
					break;
			}
			p = j;
			if(temp!="" && temp != "\n" && temp != "\t")
				v.push_back(temp);
		}
		temp = v[v.size()-1];
		vector<string> vec;
		for(int pp = 0; pp<v.size()-1; pp++){
			types[v[pp]] = temp;
		}
	}
	if(a.find('=') != string::npos && a.find("array [") != string::npos){//ARRAYS
		v.clear();
		string temp;
		int p,j;
		for(p = 0; p<a.size();p++){
			temp = "";
			for(j = p;j<a.size();j++){
				if(a[j]!=' ' && a[j] != '=' && a[j] != ',' && a[j] != '\t' && a[j] != ';' && a[j] != '\n'){
					temp.push_back(a[j]);
				}
				else
					break;
			}
			p = j;
			if(temp!="" && temp != "\n" && temp != "\t")
				v.push_back(temp);
		}
		temp = v[v.size()-1];
		pair<pair<int, int>, string> tp;
		tp.second = temp;
		int size1, size2;
		string tp1 = v[v.size() - 3];
		string tp2 = "";
		string tp3 = "";
		int a1, a2=0;
		for(a1 =0;a1<tp1.length();a1++){
			if(tp1[a1]=='.'){
				if(a2==1)
					break;
				else 
					a2++;
			}
			else if(tp1[a1]!='['){
				tp2.push_back(tp1[a1]);
			}
		}
		size1 = atoi(tp2.c_str());
		a1++;
		for(int u = a1; u<tp1.size()-1;u++){
			tp3.push_back(tp1[u]);
		}
		size2 = atoi(tp3.c_str());
		//cout<<temp<<endl;
		//size2 = size2-size1+1;
		tp.first.first = size1;
		tp.first.second = size2;
		for(int u = 0; u<v.size()-4;u++){
			arr[v[u]] = tp;
			//cout<<v[u]<<endl;
			//types[v[u]] = "array";
		}	
	}

	if(a.find("function") != string::npos){//FUNC DEFS
		vector<string> vec;
		v.clear();
		string temp, temp1, temp2;
		int p,j,e;
		for(p = 0; p<a.size();p++){
			temp = "";
			for(j = p;j<a.size();j++){
				if(a[j]!=' ' && a[j] != ':' && a[j] != ',' && a[j] != '\t' && a[j] != ';' && a[j] != '\n' && a[j] != '(' && a[j] != ')' && a[j] != '='){
					temp.push_back(a[j]);
				}
				else
					break;
			}
			p = j;
			if(temp!="" && temp != "\n" && temp != "\t")
				v.push_back(temp);
		}
		temp = v[0];
		temp1 = v[1];
		temp2 = v[v.size()-1];
		string temp3 = "";
		int flag = 0;
		for(p = 0; p<a.size();p++){
			if(a[p] == '('){
				for(e = p+1;e<a.size();e++){
					if(a[e]!=')')
						temp3.push_back(a[e]);
					else{
						flag = 1;
						break;
					}
				}
			}
			if(flag == 1)
				break;
		}
		temp3 += ";";
		//cout<<temp3<<endl;
		string temp4 = "";
		sname = temp1;
		vec.push_back(temp1);
		//cout<<sname<<endl;
		//cout<<temp3<<endl;
		for(p =0;p<temp3.size();p++){
			if(temp3[p] ==';'){
				temp4.push_back(temp3[p]);
				//cout<<temp4<<endl;
				parse(temp4, 2); // for declarations inside a function
				temp4 = "";
			}
			else{
				temp4.push_back(temp3[p]);
			}
		}
		variables2[sname] = sname;
		funcret[sname] = temp2;
	}
}

void se(){//STRUCT EQUIV
	for(int i =0; i< matdim;i++){
		for(int j = 0; j<matdim;j++){
			if(matrix[i][j]==0){
				if(comp(mat[i],mat[j])== 0){
					matrix[i][j]=1;
				}
			}
		}
	}
}

int comp(string a, string b){//COMP VARS
	//a = variables[a];
	//b = variables[;
	//cout<<a<<" "<<b<<endl;
	if(variables2.find(a)!=variables2.end()){
		a = variables2[a];
	}
	if(variables2.find(b)!=variables2.end()){
		b = variables2[b];
	}
	while(types.find(a)!=types.end()){
		a = types[a];
	}
	while(types.find(b)!=types.end()){
		b = types[b];
	}
	if(structures.find(a)!=structures.end() && structures.find(b)!=structures.end()){
		vector<string> va, vb;
		va.clear();
		vb.clear();
		for(int n = 0; n<str2[b].size();n++){
			vb.push_back(str2[b][n]);
		}
		for(int n = 0; n<str2[a].size();n++){
			va.push_back(str2[a][n]);
		}
		if(va.size()!=vb.size()){
			return 0;
		}
		else{
			for(int d = 0; d< va.size(); d++){
				//cout<<va[d]<<" "<<vb[d]<<endl;
				if(comp(va[d],vb[d])==0)
					return 0;
			}
			return 1;
		}
	}
	else if(func.find(a)!=func.end() && func.find(b)!=func.end()){
		if(funcret[a]!=funcret[b]){
			return 0;
		}
		else{
			vector<string> va, vb;
			va.clear();
			vb.clear();
			vb.clear();
			for(int n = 0; n<fun2[b].size();n++){
				vb.push_back(fun2[b][n]);
			}
			for(int n = 0; n<fun2[a].size();n++){
				va.push_back(fun2[a][n]);
			}
			if(va.size()!=vb.size()){
				return 0;
			}
			else{
				for(int d = 0; d< va.size(); d++){

					if(comp(va[d],vb[d])==0)
						return 0;
				}
				return 1;
			}
		}
	}
	else if(a.find('^')!= string::npos && b.find('^')!=string::npos){
		string t1="", t2="";
		for(int w = 1; w<a.length();w++){
			t1.push_back(a[w]);
		}
		for(int w = 1; w<b.length();w++){
			t2.push_back(b[w]);
		}
		if(comp(t1,t2) == 1){
			return 1;
		}
		else
			return 0;
	}
	else if(arr.find(a)!=arr.end() && arr.find(b)!=arr.end()){
		if((arr[a].first) != (arr[b].first))
			return 0;
		if(comp((arr[a].second),(arr[b].second))==0){
			return 0;
		}
		return 1;
	}
	else{
		if(a == b){
			//cout<<a<<" "<<b<<endl;
			return 1;
		}
		else{
			return 0;
		}
	}
}

int ne(string a, string b){
	if(variables2[a]==variables2[b])
		return 1;
	else
		return 10;
}

int ine(string a, string b){
	for(int i = 0;i<intnequi.size();i++){
		if(find(intnequi[i].begin(),intnequi[i].end(),a)!=intnequi[i].end() && find(intnequi[i].begin(),intnequi[i].end(),b)!=intnequi[i].end())
			return 2;
	}
	return 20;
}
int chse(string a, string b){
	int ia, ib;
	for(ia = 0;ia<mat.size();ia++){
		if(mat[ia]==a)
			break;
	}
	for(ib = 0;ib<mat.size();ib++){
		if(mat[ib]==b)
			break;
	}
	if(matrix[ia][ib]==1)
		return 3;
	else
		return 30;
}