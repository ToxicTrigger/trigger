#ifndef ZINNY_H
#define ZINNY_H

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

#ifdef _WIN64
#include <direct.h>
#define GetCurrentDir _getcwd
#define uint uint64_t
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define ARCHIVE_NAME ".pot"

/*
readable + version + sha256_key + file_list + file_size_list + files
*/

class zinny
{
  public:
	int version;

  private:
	uint size;
	std::string name;
	bool readable;
	std::vector<std::string> command;

	uint skip(std::ifstream *in)
	{
		bool readable;
		in->read(reinterpret_cast<char *>(&readable), sizeof(bool));
		int version;
		in->read(reinterpret_cast<char *>(&version), sizeof(int));
		uint size;
		in->read(reinterpret_cast<char *>(&size), sizeof(uint));
		return size;
	}

  public:
	zinny(std::vector<std::string> command)
	{
		this->version = 1;
		this->readable = true;
		this->name = command[1] + ARCHIVE_NAME;
		this->command = command;
	}

	//TODO Directory pack
	bool packing()
	{
		std::ofstream out;
		out.open(name, std::ios::binary);

		uint count = this->command.size();
		if(this->command[this->command.size()-1] == "-ur")
		{
			this->readable = false;
			count --;
		}

		out.write(reinterpret_cast<const char*>(&this->readable), sizeof(bool));
		out.write(reinterpret_cast<const char*>(&this->version), sizeof(int));
		uint tmp = count - 2;
		out.write(reinterpret_cast<const char*>(&tmp), sizeof(uint));

		for(int i = 2; i < count ; ++i)
		{
			std::ifstream in(this->command[i], std::ios::binary);
			if(!in.is_open())
			{
				std::cout << "Can't Find : " << this->command[i] << std::endl;
				break;
			}
			//file_name
			out.write(this->command[i].c_str(), this->command[i].size());
			out.write("\0", sizeof(char));
			in.seekg(0, std::ios::end);
			this->size += in.tellg();
			std::cout << this->command[i] << "\t\t\tReaded : " << in.tellg() << std::endl;
			in.close();
		}

		for(int i = 2; i < count ; ++i)
		{
			std::ifstream in(this->command[i], std::ios::binary);
			if(!in.is_open())
			{
				std::cout << "Can't Find : " << this->command[i] << std::endl;
				break;
			}
			//file_size
			in.seekg(0, std::ios::end);
			uint size = in.tellg();
			out.write(reinterpret_cast<const char *>(&size), sizeof(uint));
			in.close();
		}

		for(int i = 2; i < count ; ++i)
		{
			std::ifstream in(this->command[i], std::ios::binary);
			if(!in.is_open())
			{
				std::cout << "Can't Find : " << this->command[i] << std::endl;
				break;
			}
			out << in.rdbuf();
			std::cout << "\t- " << this->command[i] << " Packed" << std::endl;
			in.close();
		}

		out.close();
		std::cout << std::endl;
		std::cout << this->name << " Packaged." << std::endl;
		std::cout << std::endl;
		return true;
	}

	void view()
	{
		std::ifstream in(this->command[1], std::ios::binary);

		bool readable;
		in.read(reinterpret_cast<char *>(&readable), sizeof(bool));
		std::cout << "Readable \t: " << readable << std::endl;

		int version;
		in.read(reinterpret_cast<char *>(&version), sizeof(int));
		std::cout << "Version \t: " << version << std::endl;
		if(!readable)
		{
			std::cout << "No more infomation \t: " << this->command[1] << std::endl;
			return;
		}
		uint size;
		in.read(reinterpret_cast<char *>(&size), sizeof(uint));
		std::cout << "Files \t\t: " << size << std::endl;

		std::vector<std::string> filenames = std::vector<std::string>();
		for (int i = 0; i < size; ++i)
		{
			std::string name;
			std::getline(in, name, '\0');
			filenames.push_back(name);
		}
		for (int i = 0; i < size; ++i)
		{
			uint ssize;
			in.read(reinterpret_cast<char *>(&ssize), sizeof(uint));
			std::cout << "\tname : " << filenames[i] << std::endl;
			std::cout << "\t- " << ssize << " bytes" << std::endl;
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	void unpack()
	{
		std::ifstream in(this->command[1], std::ios::binary);
		if (!in.is_open())
		{
			std::cout << "Fail!" << std::endl;
			std::cout << "Can't Find " << this->command[1] << std::endl;
			return;
		}
		std::string out_path;
		char dir[1000];
		GetCurrentDir(dir, 1000);
		out_path = dir;

		std::string slash;

#ifdef _WIN64
		slash = "\\";
#else
		slash += "/";
#endif

		out_path += slash;

		std::string pot_name = this->command[1];
		pot_name.resize(pot_name.size() - 4);

		if (this->command.size() > 2)
		{
			std::cout << " test " << this->command[2] << std::endl;
			out_path += this->command[2];
			out_path += pot_name;
		}
		else
		{
			out_path += pot_name;
		}

		const int dir_err = system(("mkdir " + out_path).c_str());
		if (dir_err == -1)
		{
			std::cout << "Fail : " << out_path << std::endl;
		}
		else
		{
			std::cout << "Out Path : " << out_path << std::endl;
		}

		uint size = this->skip(&in);

		//1. load file size
		std::vector<std::string> filenames = std::vector<std::string>();
		for (int i = 0; i < size; ++i)
		{
			std::string name;
			std::getline(in, name, '\0');
			filenames.push_back(name);
		}
		std::vector<uint> file_size = std::vector<uint>();
		for (int i = 0; i < size; ++i)
		{
			uint ssize;
			in.read(reinterpret_cast<char *>(&ssize), sizeof(uint));
			std::cout << "\tname : " << filenames[i] << std::endl;
			std::cout << "\t- " << ssize << " bytes" << std::endl;
			std::cout << std::endl;
			file_size.push_back(ssize);
		}
		std::cout << "---------------------------------" << std::endl;
		for (int i = 0; i < size; ++i)
		{
			uint ssize = file_size[i];
			std::string real_name = out_path + slash + filenames[i].substr(filenames[i].rfind(slash.c_str()) + 1);
			std::ofstream out(real_name, std::ios::binary);

			std::cout << "OUT : " << real_name << std::endl;

			char *buf = new char[ssize];
			in.read(reinterpret_cast<char *>(buf), ssize);
			out.write(reinterpret_cast<const char *>(buf), ssize);

			out.close();
		}
		std::cout << "---------------------------------" << std::endl;
		std::cout << "un-packaging " << out_path << std::endl;
		std::cout << std::endl;
	}

	int open(std::string path)
	{
		if (path.empty())
			return 0;

		std::ifstream fin;
		fin.open(path, std::ios::binary);
		fin.seekg(0, std::ios::end);

		int size = fin.tellg();

		fin.seekg(0, std::ios::beg);
		this->size = size;
		std::cout << "sucsess : " << this->size << " byte are readed!" << std::endl
				  << std::endl;
		return size;
	}

	void init()
	{
		std::string out_path;
		std::string command = "mkdir ";
		std::string slash;

#ifdef _WIN64
		slash = "\\";
#else
		slash += "/";
#endif

		char dir[1000];
		GetCurrentDir(dir, 1000);

		out_path = dir;
		out_path += slash;
		system((command + out_path + this->command[1]).c_str());
		out_path += this->command[1];

		system((command + out_path + slash + "Assets").c_str());
		system((command + out_path + slash + "Build").c_str());
		system((command + out_path + slash + "ProjectSetting").c_str());
		//TODO TOML Parse

		std::cout << "init done" << std::endl
				  << std::endl;
	}

	bool check()
	{
		uint number = this->command.size();
		bool query_done = true;
		for (int x = 1; x < number; ++x)
		{
			std::ifstream in(this->command[x], std::ios::binary);
			if (!in.is_open())
			{
				query_done = false;
				std::cout << "\t- Can't Find " << this->command[x] << std::endl;
				break;
			}
			in.seekg(0, std::ios::end);
			//check pot size
			uint pot_size = in.tellg();
			in.seekg(0, std::ios::beg);

			uint real_size = 0;

			bool readable;
			in.read(reinterpret_cast<char *>(&readable), sizeof(bool));
			real_size += in.tellg();
			int version;
			in.read(reinterpret_cast<char *>(&version), sizeof(int));
			real_size += (uint)in.tellg() - real_size;
			uint size;
			in.read(reinterpret_cast<char *>(&size), sizeof(uint));
			real_size += (uint)in.tellg() - real_size;

			//check pot all files size
			std::vector<std::string> filenames = std::vector<std::string>();
			for (int i = 0; i < size; ++i)
			{
				std::string name;
				std::getline(in, name, '\0');
				real_size += (uint)in.tellg() - real_size;
				filenames.push_back(name);
			}
			std::vector<uint> file_size = std::vector<uint>();
			for (int i = 0; i < size; ++i)
			{
				uint ssize;
				in.read(reinterpret_cast<char *>(&ssize), sizeof(uint));
				real_size += (uint)in.tellg() - real_size;
				file_size.push_back(ssize);
			}

			for (int i = 0; i < size; ++i)
			{
				uint ssize = file_size[i];
				char *buf = new char[ssize];
				in.read(reinterpret_cast<char *>(buf), ssize);
				real_size += (uint)in.tellg() - real_size;
			}

			if (real_size != pot_size)
			{
				std::cout << "---------------------------------" << std::endl;
				std::cout << this->command[x] << " is Broken!" << std::endl;
				std::cout << "Real : " << real_size << std::endl;
				std::cout << "pot : " << pot_size << std::endl;
				std::cout << std::endl;
				query_done = false;
				break;
			}

			std::cout << "Un-Packageable File : " << this->command[x] << std::endl;
		}
		return query_done;
	}
};

#endif