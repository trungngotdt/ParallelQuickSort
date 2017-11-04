
/*
Ý tưởng :sử dụng phương pháp chia để trị

1/Lấy một phần tử của mảng làm chốt
2/Dựa vào chốt chia mảng làm hai phần sao cho các phần tử bên trái bé hơn chốt và các phần tử bên phải lớn hơn chốt
3/Sắp xếp mảng trái
5/Sắp xếp mảng phải
Cứ tiếp tục như vậy cho đến khi nào sắp xếp xong
*/
#include <stdio.h>
#include<cilk\cilk.h>
#include<list>
#include<cstdlib>
#include <conio.h>
#include<time.h>
#include<ctime>
#include<vector>
void ParaQuicksort(unsigned short *A, int l, int r)
{
	if (l >= r) return;
	int i = l;
	int j = r;
	//phần tử chốt chọn bằng cách (l+r)/2 là chỉ số của A hay ta có chốt là A[(l + r) / 2]
	int x = A[(l + r) / 2];
	//Quá trình lặp này sẽ chọn ra một mảng bé hơn chốt và một mảng có các phần tử lớn hơn chốt 
	//Cho i chạy từ trái sang phải
	//Cho j chạy từ phải sang trái
	//Một đường từ đầu danh sách, một đường từ cuối danh sách.
	//Theo cách này, ta tìm phần tử đầu tiên tính từ trái lớn hơn phần tử chốt và phần tử đầu tiên phía phải nhỏ hơn hoặc bằng phần tử chốt rồi đổi chỗ cho nhau.
	//Tiếp tục như vậy cho đến khi hai đường gặp nhau.
	while (i <= j)
	{
		//Tìm phần tử bên trái mà lớn hơn hay bằng chốt
		//Duyệt từ trái sang phải dừng lại khi bắt gặp phần tử nào đó lớn hơn hoặc bằng chốt
		//Đây sẽ làm cho mảng bến trái bé hơn chốt
		while (A[i] < x)
		{
			i++;
		}
		//Tìm phần tử bên phải mà bé hơn hay bằng chốt
		//Duyệt từ phải sang trái dừng lại khi bắt gặp phần tử nào đó bé hơn hoặc bằng chốt
		//Đây sẽ làm mảng bên phải lớn hơn chốt
		while (A[j] > x)
		{
			j--;
		}
		//Đây là hàm trao đổi hay swap
		//Đổi cho 2 phần tử kiểu int A[i], A[j].Nghĩa là đổi chỗ hai phần tử lớn hơn hoặc bằng với bé hơn hoặc bằng chốt
		//Đảm bảo cho bên trái nhỏ hơn chốt và bên phải lớn hơn chốt
		if (i <= j)
		{
			int temp = A[i];
			A[i] = A[j];
			A[j] = temp;
			i++; j--;
		}
	}
	//Sau vòng lặp ta sẽ được bên trái là các phần tử bé hơn hoặc bằng chốt và bên phải là các phần tử lớn hoặc bằng chốt
	//Đệ quy sắp xếp cho đến khi nào không thể chia nhỏ được nữa thì thôi.
	//Cứ tiếp tục sắp xếp như vậy với 2 dãy đã chia thì sẽ sắp xếp được cả dãy.
	cilk_spawn ParaQuicksort(A, l, j);//Mảng bé hơn hoặc bằng chốt .Không thể chia nhỏ khi l>=j
	ParaQuicksort(A, i, r);//Mảng lớn hơn hoặc bằng chốt .Không thể chia nhỏ khi i>=r
	cilk_sync;
}
void Quicksort(unsigned short *A, int l, int r)
{
	if (l >= r) return;
	int i = l;
	int j = r;
	int x = A[(l + r) / 2];
	while (i <= j)
	{
		while (A[i] < x)
		{
			i++;
		}
		while (A[j] > x)
		{
			j--;
		}
		if (i <= j)
		{
			int temp = A[i];
			A[i] = A[j];
			A[j] = temp;
			i++; j--;
		}
	}
	Quicksort(A, l, j);
	Quicksort(A, i, r);

}
int main()
{
	int i;
	int len = 1026531840;
	//int len = 402653184;
	unsigned short *A = new unsigned short[len];
	unsigned short *B = new unsigned short[len];
	//Cần 1610612736 byte cho mảng có 402653184 phần tử
	//int* A = (int*)malloc(len * sizeof(int)); //Cấp phát bộ nhớ cho mảng
	//int* B = (int*)malloc(len * sizeof(int));//Cấp phát bộ nhớ cho mảng
											 /*Vì khối nhớ này sẽ được trỏ đến bởi A, nên chúng ta phải ép kiểu
											 trước hàm malloc là(int*)
											 Nếu câu lệnh thành công, thì A sẽ là con trỏ đến ô nhớ đầu tiên trong
											 khối nhớ.
											 Đối với B tương tự
											 */
	printf_s("Bat dau quicksort mang %d\n", len);
	for (i = 0; i < len; i++)
	{
		int cc = rand();
		A[i]=B[i] = rand() + i*cc + rand() / 2 + 1 + rand() / 3 + cc*i / 2;
	}
	printf("\tNap xong mang\n");
	//tính giờ
	clock_t begin = clock();
	ParaQuicksort(A, 0, len - 1);
	clock_t end = clock();
	printf_s("Thoi gian chay song song %f time \n", (float)(end - begin) / CLOCKS_PER_SEC);
	//-------------------------
	clock_t begin1 = clock();
	Quicksort(B, 0, len - 1);
	clock_t end1 = clock();
	printf_s("Thoi gian chay binh thuong %f time \n", (float)(end1 - begin1) / CLOCKS_PER_SEC);
	//-------------------------
	//Giải phóng bộ nhớ
	free(A);
	//free(B);
	getch();
}