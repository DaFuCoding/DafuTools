#ifndef TOOL_EVERYING_FIND_H_
#define TOOL_EVERYING_FIND_H_

#include <iostream>
#include <vector>
#include <string.h>

#ifdef __unix__
#include <dirent.h>
#endif

#ifdef __unix__
void find_core(std::string dirPath, std::vector<std::string>& files,
       std::vector<std::string> filter_name){
  DIR *dir;
  if ((dir=opendir(dirPath.c_str())) == NULL){
    perror("Open dir error...");
    exit(1);
  }
  struct dirent *ptr;
  std::vector<std::string> folders;
  while ((ptr=readdir(dir)) != NULL){
    if(strcmp(ptr->d_name,".")==DT_UNKNOWN || strcmp(ptr->d_name,"..")==DT_UNKNOWN)    ///current dir OR parrent dir
      continue;
    else if(ptr->d_type == DT_REG){    ///file
      for(int i=0;i<filter_name.size();++i){
        if(strcmp(ptr->d_name + strlen(ptr->d_name)-3,filter_name[i].c_str()) == 0){
          printf("file_name:%s\n",ptr->d_name);
          files.push_back(dirPath + ptr->d_name);
          break;
        }
      }
    }else if(ptr->d_type == DT_DIR){    ///dir
      folders.push_back(ptr->d_name);
      //printf("dir_name:%s\n",ptr->d_name);
    }
  }
  // recursion find file
  for(int i=0;i<folders.size();i++){
    std::string newDir = dirPath+folders[i]+"/";
    find_core(newDir,files,filter_name);
  }
  closedir(dir);
}
/**
 * @brief find_file: recursion
 * @param dirPath : root/start path
 * @param files : return all files absolute path
 * @param filter: select name, separate different name using a non-alpha char(' ',',')
 */
void find_file(std::string dirPath, std::vector<std::string>& files,
       std::string filter){
  if(dirPath[dirPath.size()-1] !='/') dirPath += '/';
  // handle filter
  std::vector<std::string> filter_name;
  for(int i=0;i<filter.size();++i){
    std::string small="",large="";
    while(isalpha(filter[i])){
      small += tolower(filter[i]);
      large += toupper(filter[i]);
      ++i;
    }
    if(small.size()) {
      filter_name.push_back(small);
      filter_name.push_back(large);
    }
  }
  find_core(dirPath,files,filter_name);
}

#endif
#endif
//int main(){
//  std::vector<std::string> files;
//  find_file("/home/dafu/workspace/",files,"avi mp4");
//  std::cout<<files.size();
//  return 0;
//}
