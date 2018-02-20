#include <string.h>
#include <stdio.h>

void deletecomment(char *buff, unsigned buffsize);

int readfile(char *filename, unsigned buffsize);

unsigned sizefile(FILE *file);

int main(int argc, char const *argv[])
{
	char filename[64];
	unsigned buffsize = 128;
	do
	{
		printf("Введите название файла с расширением:\n");
		scanf("%s", filename);
	}
	while(readfile(filename, buffsize));
	while(readfile(filename, buffsize));
	printf("Готово!\n");
	return 0;
}

int readfile(char *filename, unsigned buffsize)
{
	FILE *f1, *f2;
	unsigned size = 0, chunk_size = 0;
	char *buff;
	if((f1 = fopen(filename, "r")) != NULL)
	{
		f2 = fopen("nocomments", "w");
		buff = new char[buffsize--];
		size = sizefile(f1);
		chunk_size = ((size / buffsize) > 0)?buffsize:(((size % buffsize) > 0)?(size % buffsize):0);
		size -= chunk_size;
		buff[chunk_size] = '\0';
		while(chunk_size > 0)
		{
			fread(buff, sizeof(char), chunk_size, f1);
			deletecomment(buff, chunk_size);
			fwrite(buff, sizeof(char), strlen(buff), f2);
			chunk_size = ((size / buffsize) > 0)?buffsize:(((size % buffsize) > 0)?(size % buffsize):0);
			buff[chunk_size] = '\0';
			size -= chunk_size;
		}
		fclose(f1);
		fclose(f2);
		delete[] buff;
		if(remove(filename) == 0)
		{
			rename("nocomments", filename);
		}
		return 0;
	}
	printf("Невозможно окрыть файл!\n");
	return 1;
}

void deletecomment(char *buff, unsigned buffsize)
{
	static int n1 = 0, n2 = 0;
	static char last_char = ' ';
	int
		length1 = 0,
		length2 = 0,
		last = buffsize;
	for(int x = 0; x < last; ++x)
	{
		if(((buff[x] == '/') && (buff[x + 1] == '*')) || ((last_char == '/') && (buff[0] == '*')))
		{
			n2 = 1;
		}
		if(n2 == 1)
		{
			length2++;
			if(((buff[x] == '*') && (buff[x + 1] == '/')) || ((last_char == '*') && (buff[0] =='/')))
			{
				length2++;
				x += 1;
				for(int j = x; j <= buffsize; ++j)
				{
					buff[j - length2] = buff[j];
				}
				x -= length2;
				last -= length2;
				length2 = 0;
				n2 = 0;
			}
		}
		else//////////////////////////////////////////////////////////////////////////////////////////
		{
			if(((buff[x] == '/') && (buff[x + 1] == '/')) || ((buff[0] == '/') && (last_char == '/')))
			{
				n1 = 1;
			}
			if(n1 == 1)
			{
				length1++;
				if(buff[x] == '\n')
				{
					length1--;
					for(int j = x; j <= buffsize; ++j)
					{
						buff[j - length1] = buff[j];
					}
					x -= length1;
					last -= length1;
					length1 = 0;
					n1 = 0;
				}
			}
		}
		if(x == last)
		{
			last_char = buff[x];
		}
	}
	if(n1 == 1)
	{
		last -= length1;
		buff[last] = '\0';
	}
	if(n2 == 1)
	{
		last -= length2;
		buff[last] = '\0';
	}
	
}

unsigned sizefile(FILE *file)
{
	unsigned size = 0;
	while(fgetc(file) != EOF)
	{
		size++;
	}
	fseek(file, 0, SEEK_SET);
	return size;
}