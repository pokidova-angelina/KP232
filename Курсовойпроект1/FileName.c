#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <locale.h>
#define M_SIZE 100

struct  Blog
{
    char title[100];
    char author[50];
    char category[50];
    char publication_date[20];
    int comment;
    double rating;
};
int display();
int searchByTitle(struct Blog* blog, int size, char* searchTitle);
int searchByCategory(struct Blog* blog, int size, char* searchCategory, int* resultIndices);
void sortByRating(struct Blog* blog, int size);
int writeToFile(struct Blog* blog, int size);
int readToFile(struct Blog* blog, int size);
int add_to_array(struct Blog* blog, int size);
int displayBlog(struct Blog* blog, int size);

int main()
{
    setlocale(LC_ALL, "RUS");
    system("chcp 1251");
    struct Blog bloges[M_SIZE];
    int blogCount = 0;
    int choice;
    do
    {
        display();
        printf("������� ��� �����: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
            blogCount = add_to_array(bloges, blogCount);
            break;
        case 2:
            if (blogCount == 0)
            {
                printf("��� ��������� ������.\n");
            }
            else
            {
                char searchTitle[50];
                printf("������� ��������� ����� ��� ������: ");
                fgets(searchTitle, sizeof(searchTitle), stdin);
                int index = searchByTitle(bloges, blogCount, searchTitle);
                if (index != -1)
                {
                    printf("\n���������� ������ �� �������� %s", searchTitle);
                    displayBlog(&bloges[index], 1);
                }
            }
            break;
        case 3:
            if (blogCount == 0)
            {
                printf("��� ��������� ������.\n");
            }
            else
            {
                char searchCategory[50];
                printf("������� ��������� ����� ��� ������: ");
                fgets(searchCategory, sizeof(searchCategory), stdin);
                int resultIndices[M_SIZE];
                int resultCount = searchByCategory(bloges, blogCount, searchCategory, resultIndices);
                if (resultCount > 0)
                {
                    printf("\n���������� ������ �� ��������� %s", searchCategory);
                    for (int i = 0; i < resultCount; ++i)
                    {
                        displayBlog(&bloges[resultIndices[i]], 1);
                    }
                }
                else
                {
                    printf("��� ����������� ��� ��������� ���������.\n");
                }
            }
            break;
        case 4:
            if (blogCount > 0)
            {
                sortByRating(bloges, blogCount);
                printf("\n��������������� ������ �� �������� �� ��������:\n");
                displayBlog(bloges, blogCount);
            }
            else {
                printf("���� ������ �����.\n");
            }
            break;
        case 5:
            if (blogCount > 0)
            {
                int res = writeToFile(bloges, blogCount);
                if (res == 0)
                {
                    printf("�� ������� �������� ������ � ����.\n\n");
                }
                else
                {
                    printf("������ ������� �������� � ����.\n");
                }
            }
            else
            {
                printf("���� ������ �����. ������ ���������� � ����.\n");
            }
            break;
        case 6:
            if (blogCount > 0)
            {
                int res = readToFile(bloges, blogCount);
                if (res == 0)
                {
                    printf("������ ������� ��������� �� �����.\n");
                }
                else
                {
                    printf("�� ������� ��������� ������ �� �����.\n");
                }
            }
            else
            {
                printf("���� ������ �����. ������ ������ �� �����.\n");
            }
            break;
        case 7:
            printf("��������� ���������.\n");
            break;
        default:
            printf("������������ ����. ����������, �������� ������������ ����� ����.\n");
        }

    } while (choice != 7);
}
int display()
{
    printf("����� ���������� � ���� ��� ���������� �������!\n");
    printf("-----------------------------------------------------------\n");
    printf("����:\n");
    printf("1. �������� ����\n");
    printf("2. ����� ����� �� ��������� ������\n");
    printf("3. ����� ����� �� ��������� ������\n");
    printf("4. ���������� �� ��������\n");
    printf("5. ������ � ����\n");
    printf("6. ������ �����\n");
    printf("7. ����� �� ���������\n");
    printf("-----------------------------------------------------------\n");
}
int searchByTitle(struct Blog* blog, int size, char* searchTitle)
{
    for (int i = 0; i < size; ++i)
    {
        if (strstr(blog[i].title, searchTitle) != NULL)
        {
            return i;
        }
    }
    return -1;
}

int searchByCategory(struct Blog* blog, int size, char* searchCategory, int* resultIndices)
{
    int count = 0;
    for (int i = 0; i < size; ++i)
    {
        if (strstr(blog[i].category, searchCategory) != NULL)
        {
            resultIndices[count++] = i;
        }
    }
    return count;
}

void sortByRating(struct Blog* blog, int size)
{
    for (int i = 0; i < size - 1; ++i)
    {
        for (int j = 0; j < size - i - 1; ++j)
        {
            if (blog[j].rating < blog[j + 1].rating)
            {
                struct Blog temp = blog[j];
                blog[j] = blog[j + 1];
                blog[j + 1] = temp;
            }

        }
    }
}

int readToFile(struct Blog* blog, int size)
{
    FILE* file;
    char filename[] = "blog.txt";
    char buf[1000];
    file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("������ ��� �������� �����");
        return 1;
    }
    while (fgets(buf, sizeof(buf), file) != NULL)
    {
        printf("%s", buf);
    }
    fclose(file);
    return 0;
}

int writeToFile(struct Blog* blog, int size)
{
    FILE* file = fopen("blog.txt", "w");
    if (file == NULL)
    {
        return 0;
    }
    for (int i = 0; i < size; ++i)
    {
        fprintf(file, "���������: %s\n", blog[i].title);
        fprintf(file, "�����: %s\n", blog[i].author);
        fprintf(file, "���������: %s\n", blog[i].category);
        fprintf(file, "����: %s\n", blog[i].publication_date);
        fprintf(file, "���������� ������������: %d\n", blog[i].comment);
        fprintf(file, "�������: %.2f\n", blog[i].rating);
    }
    fclose(file);
    return 1;
}

int displayBlog(struct Blog* blog, int size)
{
    for (int i = 0; i < size; ++i)
    {
        printf("------------------------------------------------\n");
        printf("��������� ������: %s", blog[i].title);
        printf("��� ������: %s", blog[i].author);
        printf("��������� ������: %s", blog[i].category);
        printf("���� ����������: %s", blog[i].publication_date);
        printf("���������� ������������: %d\n", blog[i].comment);
        printf("�������: %.2lf\n", blog[i].rating);
        printf("------------------------------------------------\n");
    }
    return size;
}

int add_to_array(struct Blog* blog, int size)
{
    if (size < M_SIZE)
    {
        printf("��������� ������: ");
        fgets(blog[size].title, sizeof(blog->title), stdin);
        printf("��� ������: ");
        fgets(blog[size].author, sizeof(blog->author), stdin);
        printf("��������� ������: ");
        fgets(blog[size].category, sizeof(blog->category), stdin);
        printf("���� ����������: ");
        fgets(blog[size].publication_date, sizeof(blog->publication_date), stdin);
        printf("���������� ������������: ");
        scanf("%d", &blog[size].comment);
        printf("�������: ");
        scanf("%lf", &blog[size].rating);
        return size + 1;
    }
    else
    {
        printf("���������� ������������ ���������� ������!\n");
        return size;
    }
}
