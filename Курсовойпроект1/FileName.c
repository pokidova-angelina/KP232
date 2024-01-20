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
        printf("Введите ваш выбор: ");
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
                printf("Нет доступных блогов.\n");
            }
            else
            {
                char searchTitle[50];
                printf("Введите заголовок блога для поиска: ");
                fgets(searchTitle, sizeof(searchTitle), stdin);
                int index = searchByTitle(bloges, blogCount, searchTitle);
                if (index != -1)
                {
                    printf("\nРезультаты поиска по названию %s", searchTitle);
                    displayBlog(&bloges[index], 1);
                }
            }
            break;
        case 3:
            if (blogCount == 0)
            {
                printf("Нет доступных блогов.\n");
            }
            else
            {
                char searchCategory[50];
                printf("Введите категорию блога для поиска: ");
                fgets(searchCategory, sizeof(searchCategory), stdin);
                int resultIndices[M_SIZE];
                int resultCount = searchByCategory(bloges, blogCount, searchCategory, resultIndices);
                if (resultCount > 0)
                {
                    printf("\nРезультаты поиска по категории %s", searchCategory);
                    for (int i = 0; i < resultCount; ++i)
                    {
                        displayBlog(&bloges[resultIndices[i]], 1);
                    }
                }
                else
                {
                    printf("Нет результатов для указанной категории.\n");
                }
            }
            break;
        case 4:
            if (blogCount > 0)
            {
                sortByRating(bloges, blogCount);
                printf("\nОтсортированные данные по рейтингу по убыванию:\n");
                displayBlog(bloges, blogCount);
            }
            else {
                printf("База данных пуста.\n");
            }
            break;
        case 5:
            if (blogCount > 0)
            {
                int res = writeToFile(bloges, blogCount);
                if (res == 0)
                {
                    printf("Не удалось записать данные в файл.\n\n");
                }
                else
                {
                    printf("Данные успешно записаны в файл.\n");
                }
            }
            else
            {
                printf("База данных пуста. Нечего записывать в файл.\n");
            }
            break;
        case 6:
            if (blogCount > 0)
            {
                int res = readToFile(bloges, blogCount);
                if (res == 0)
                {
                    printf("Данные успешно прочитаны из файла.\n");
                }
                else
                {
                    printf("Не удалось прочитать данные из файла.\n");
                }
            }
            else
            {
                printf("База данных пуста. Нечего читать из файла.\n");
            }
            break;
        case 7:
            printf("Программа завершена.\n");
            break;
        default:
            printf("Некорректный ввод. Пожалуйста, выберите существующий пункт меню.\n");
        }

    } while (choice != 7);
}
int display()
{
    printf("Добро пожаловать в меню для управления блогами!\n");
    printf("-----------------------------------------------------------\n");
    printf("Меню:\n");
    printf("1. Добавить блог\n");
    printf("2. Поиск блога по заголовку статьи\n");
    printf("3. Поиск блога по категории статьи\n");
    printf("4. Сортировка по рейтингу\n");
    printf("5. Запись в файл\n");
    printf("6. Чтение файла\n");
    printf("7. Выйти из программы\n");
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
        printf("Ошибка при открытии файла");
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
        fprintf(file, "Заголовок: %s\n", blog[i].title);
        fprintf(file, "Автор: %s\n", blog[i].author);
        fprintf(file, "Категория: %s\n", blog[i].category);
        fprintf(file, "Дата: %s\n", blog[i].publication_date);
        fprintf(file, "Количество комментариев: %d\n", blog[i].comment);
        fprintf(file, "Рейтинг: %.2f\n", blog[i].rating);
    }
    fclose(file);
    return 1;
}

int displayBlog(struct Blog* blog, int size)
{
    for (int i = 0; i < size; ++i)
    {
        printf("------------------------------------------------\n");
        printf("Заголовок статьи: %s", blog[i].title);
        printf("Имя автора: %s", blog[i].author);
        printf("Категория статьи: %s", blog[i].category);
        printf("Дата публикации: %s", blog[i].publication_date);
        printf("Количество комментариев: %d\n", blog[i].comment);
        printf("Рейтинг: %.2lf\n", blog[i].rating);
        printf("------------------------------------------------\n");
    }
    return size;
}

int add_to_array(struct Blog* blog, int size)
{
    if (size < M_SIZE)
    {
        printf("Заголовок статьи: ");
        fgets(blog[size].title, sizeof(blog->title), stdin);
        printf("Имя автора: ");
        fgets(blog[size].author, sizeof(blog->author), stdin);
        printf("Категория статьи: ");
        fgets(blog[size].category, sizeof(blog->category), stdin);
        printf("Дата публикации: ");
        fgets(blog[size].publication_date, sizeof(blog->publication_date), stdin);
        printf("Количество комментариев: ");
        scanf("%d", &blog[size].comment);
        printf("Рейтинг: ");
        scanf("%lf", &blog[size].rating);
        return size + 1;
    }
    else
    {
        printf("Достигнуто максимальное количество курсов!\n");
        return size;
    }
}
