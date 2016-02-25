#include <stdio.h>
#include <string.h>

void stream_attr(FILE *fp, char *des)
{
	printf("%s is ", des);
	if(fp->_flags & _IO_UNBUFFERED){
		printf("unbuffered.\n");
	}else if(fp->_flags & _IO_LINE_BUF){
		printf("line buffer.\n");
	}else{
		printf("full buffer.\n");
	}
	printf("IO size is %d\n", fp->_IO_buf_end - fp->_IO_buf_base);
}

int main(int argc, char *argv[])
{
	FILE *fp;
	FILE *read_fp;
	char buf[2048];
	char read_buf[2048];

	memset(buf, 0, sizeof(buf));
	printf("Input a string:");
	scanf("%s", buf);

	stream_attr(stdin, "stdin");
	printf("===================\n");

	stream_attr(stdout, "stdout");
	printf("===================\n");

	stream_attr(stderr, "stderr");
	printf("===================\n");

	fp = fopen("/home/wdj/io_test/aaa", "w+");
	if(!fp){
		printf("open file failed.\n");
		return 0;
	}
	fprintf(fp, "%s", buf);
	stream_attr(fp, "aaa_fp");
	fclose(fp);
	printf("===================\n");

	read_fp = fopen("/home/wdj/io_test/aaa", "r");
	if(!fp){
		printf("open file failed.\n");
		return 0;
	}
	fscanf(fp, "%s", read_buf);
	stream_attr(read_fp, "read_fp");
	fclose(read_fp);
	return 0;
}