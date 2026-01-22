#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESCRIPTION_LENGTH 256
#define INITIAL_CAPACITY 3

typedef struct {
    int id;
    char description[MAX_DESCRIPTION_LENGTH];
    bool completed;
} TodoItem;

TodoItem *todo_list = NULL;
int todo_count = 0;
int todo_capacity = 0;
int next_id = 1;

void clean_input_buffer();
void initialize_todo_list();
void free_todo_list();
void add_todo(const char *description);
void list_todos();
void complete_todo(int id);
void delete_todo(int id);

int main(void) {
    initialize_todo_list();

    int choice;
    char temp_description[MAX_DESCRIPTION_LENGTH];
    int temp_id;

    do {
        printf("\n--- Todo List Application ---\n");
        printf("1. Add Todo\n");
        printf("2. List Todos\n");
        printf("3. Mark Todo as Complete\n");
        printf("4. Delete Todo\n");
        printf("5. Save Todos\n");
        printf("6. Exit\n");
        printf("\nEnter Your Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("\n-Invalid input!\n");
            clean_input_buffer(); // cleaning the invalid input
            continue;
        }

        clean_input_buffer(); // cleaning the return char

        switch (choice) {
        case 1:
            printf("Enter the new todo description: ");
            fgets(temp_description, sizeof(temp_description), stdin);
            temp_description[strcspn(temp_description, "\n")] = 0;
            add_todo(temp_description);
            break;

        case 2:
            list_todos();
            break;

        case 3:
            printf("\nEnter the ID of todo to complete: ");
            if (scanf("%d", &temp_id) != 1) {
                printf("\nInvalid input!\n");
                clean_input_buffer();
                continue;
            }
            clean_input_buffer();
            complete_todo(temp_id);
            break;

        case 4:
            printf("\nEnter the ID of todo to delete: ");
            if (scanf("%d", &temp_id) != 1) {
                printf("\nInvalid input!\n");
                clean_input_buffer();
                continue;
            }
            delete_todo(temp_id);
            clean_input_buffer();
            break;

        case 6:
            printf("\nExiting application. Have a great day!\n");
            break;

        default:
            printf("\nInvalid choice!\n");
        }

    } while (choice != 6);

    return 0;
}

void clean_input_buffer() {
    while (getchar() != '\n')
        ;
    return;
}

void initialize_todo_list() {
    todo_capacity = INITIAL_CAPACITY;
    todo_list = malloc(sizeof(TodoItem) * todo_capacity);
    if (!todo_list) {
        printf("\nError: failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }
}

void free_todo_list() {
    if (todo_list) {
        free(todo_list);
        todo_list = NULL;
    }

    todo_count = 0;
    todo_capacity = 0;
    next_id = 1;
}

void ensure_capacity() {
    if (todo_count >= todo_capacity) {
        todo_capacity *= 2;

        TodoItem *temp = realloc(todo_list, sizeof(TodoItem) * todo_capacity);
        if (temp == NULL) {
            printf("\nError: failed to expand the todo list\n");
            exit(EXIT_FAILURE);
        }

        todo_list = temp;
        printf("\nInfo: resized todo list to capacity of %d\n", todo_capacity);
    }

    return;
}

void add_todo(const char *description) {
    if (strlen(description) == 0) {
        printf("\nError: description cannon be empty\n");
        return;
    }

    ensure_capacity();

    TodoItem *next_item = &todo_list[todo_count];

    next_item->id = next_id++;
    snprintf(next_item->description, MAX_DESCRIPTION_LENGTH, description);
    next_item->completed = false;

    printf("\nSuccess: TodoItem added: ID %d - \"%s\"\n", next_item->id,
           next_item->description);
    todo_count++;
    return;
}

void list_todos() {
    if (todo_count == 0) {
        printf("\nThe todo list is empty.\n");
        return;
    }

    printf("\n--- Current Todo Items (%d items) ---\n", todo_count);
    for (int i = 0; i < todo_count; i++) {
        printf("ID: %-4d | Status: %-8s | Description: %s\n", todo_list[i].id,
               todo_list[i].completed ? "COMPLETE" : "PENDING",
               todo_list[i].description);
    }
    printf("----------------------------------\n");
}

void complete_todo(int id) {
    for (int i = 0; i < todo_count; i++) {
        TodoItem *next_item = &todo_list[i];

        if (next_item->id == id) {
            if (next_item->completed == true) {
                printf("\nThis item is already marked as complete!\n");
                return;
            } else {
                next_item->completed = true;
                printf("\nTodo ID %d marked as COMPLETE.\n", id);
                return;
            }
        }
    }

    printf("\nCouldn't find any todo item with this ID %d\n", id);
    return;
}

void delete_todo(int id) {
    for (int i = 0; i < todo_count; i++) {
        TodoItem *next_item = &todo_list[i];

        if (next_item->id == id) {
            int start_idx = i;

            for (int j = start_idx; j < todo_count - 1; j++) {
                todo_list[j] = todo_list[j + 1];
            }

            todo_count--;
            printf("\nSuccess: deleted todo ID %d\n", id);
            return;
        }
    }

    printf("\nError: couldn't find todo with the ID %d\n", id);
    return;
}
