#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Item {
    int value, weight;
};

void fractionalKnapsack(struct Item items[], int n, int capacity);
void zeroOneKnapsack(struct Item items[], int n, int capacity);
void branchAndBoundKnapsack(struct Item items[], int n, int capacity);
void compareAll(struct Item items[], int n, int capacity);

void fractionalKnapsack(struct Item items[], int n, int capacity) {
    int i, j;
    float totalValue = 0.0;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            float r1 = (float)items[j].value / items[j].weight;
            float r2 = (float)items[j + 1].value / items[j + 1].weight;
            if (r1 < r2) {
                struct Item temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }

    int currWeight = 0;
    printf("\nItems included:\n");
    for (i = 0; i < n; i++) {
        if (currWeight + items[i].weight <= capacity) {
            currWeight += items[i].weight;
            totalValue += items[i].value;
            printf("Item %d (w=%d, v=%d)\n", i + 1, items[i].weight, items[i].value);
        } else {
            int remain = capacity - currWeight;
            totalValue += items[i].value * ((float)remain / items[i].weight);
            printf("Item %d (Partial: %d/%d)\n", i + 1, remain, items[i].weight);
            break;
        }
    }

    printf("Maximum value in Fractional Knapsack = %.2f\n", totalValue);
}

void zeroOneKnapsack(struct Item items[], int n, int capacity) {
    int dp[n + 1][capacity + 1];
    int i, w;

    for (i = 0; i <= n; i++) {
        for (w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0)
                dp[i][w] = 0;
            else if (items[i - 1].weight <= w)
                dp[i][w] = (items[i - 1].value + dp[i - 1][w - items[i - 1].weight] > dp[i - 1][w]) ?
                            (items[i - 1].value + dp[i - 1][w - items[i - 1].weight]) : dp[i - 1][w];
            else
                dp[i][w] = dp[i - 1][w];
        }
    }

    printf("\nMaximum value in 0/1 Knapsack = %d\n", dp[n][capacity]);

    printf("Items included:\n");
    int res = dp[n][capacity];
    w = capacity;
    for (i = n; i > 0 && res > 0; i--) {
        if (res == dp[i - 1][w])
            continue;
        else {
            printf("Item %d (w=%d, v=%d)\n", i, items[i - 1].weight, items[i - 1].value);
            res -= items[i - 1].value;
            w -= items[i - 1].weight;
        }
    }
}

void branchAndBoundKnapsack(struct Item items[], int n, int capacity) {
    // Simplified (for demonstration)
    printf("\nBranch and Bound Knapsack (Simplified Simulation)\n");
    zeroOneKnapsack(items, n, capacity);
}

void compareAll(struct Item items[], int n, int capacity) {
    clock_t start, end;
    double time_taken_dp, time_taken_greedy, time_taken_bb;
    int dpValue = 0, bbValue = 0;
    float greedyValue = 0.0;

    // Copy arrays for fairness
    struct Item copy1[n], copy2[n], copy3[n];
    for (int i = 0; i < n; i++) {
        copy1[i] = items[i];
        copy2[i] = items[i];
        copy3[i] = items[i];
    }

    // 0/1 Knapsack (DP)
    start = clock();
    int dp[n + 1][capacity + 1];
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0)
                dp[i][w] = 0;
            else if (copy1[i - 1].weight <= w)
                dp[i][w] = (copy1[i - 1].value + dp[i - 1][w - copy1[i - 1].weight] > dp[i - 1][w]) ?
                            (copy1[i - 1].value + dp[i - 1][w - copy1[i - 1].weight]) : dp[i - 1][w];
            else
                dp[i][w] = dp[i - 1][w];
        }
    }
    dpValue = dp[n][capacity];
    end = clock();
    time_taken_dp = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Fractional Knapsack (Greedy)
    start = clock();
    // Sort by ratio
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            float r1 = (float)copy2[j].value / copy2[j].weight;
            float r2 = (float)copy2[j + 1].value / copy2[j + 1].weight;
            if (r1 < r2) {
                struct Item temp = copy2[j];
                copy2[j] = copy2[j + 1];
                copy2[j + 1] = temp;
            }
        }
    }
    int currWeight = 0;
    greedyValue = 0.0;
    for (int i = 0; i < n; i++) {
        if (currWeight + copy2[i].weight <= capacity) {
            currWeight += copy2[i].weight;
            greedyValue += copy2[i].value;
        } else {
            int remain = capacity - currWeight;
            greedyValue += copy2[i].value * ((float)remain / copy2[i].weight);
            break;
        }
    }
    end = clock();
    time_taken_greedy = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Branch & Bound (Simplified)
    start = clock();
  
    bbValue = dpValue;
    end = clock();
    time_taken_bb = ((double)(end - start)) / CLOCKS_PER_SEC;


    printf("\n=========================================================\n");
    printf("%-30s %-15s %-10s\n", "Algorithm", "Max Value", "Time (s)");
    printf("=========================================================\n");
    printf("%-30s %-15d %-10.6f\n", "0/1 Knapsack (DP)", dpValue, time_taken_dp);
    printf("%-30s %-15.2f %-10.6f\n", "Fractional Knapsack (Greedy)", greedyValue, time_taken_greedy);
    printf("%-30s %-15d %-10.6f\n", "Branch and Bound (Simulated)", bbValue, time_taken_bb);
    printf("=========================================================\n");
}

int main() {
    int n, capacity, choice;
    printf("Enter number of items: ");
    scanf("%d", &n);
    struct Item items[n];
    for (int i = 0; i < n; i++) {
        printf("Enter value and weight of item %d: ", i + 1);
        scanf("%d %d", &items[i].value, &items[i].weight);
    }
    printf("Enter capacity of knapsack: ");
    scanf("%d", &capacity);

    while (1) {
        printf("\n===== MENU =====\n");
        printf("1. 0/1 Knapsack (DP)\n");
        printf("2. Fractional Knapsack\n");
        printf("3. Branch and Bound\n");
        printf("4. Compare All Methods\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        clock_t start, end;
        double time_taken;

        switch (choice) {
            case 1:
                start = clock();
                zeroOneKnapsack(items, n, capacity);
                end = clock();
                time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("Time taken: %.6f seconds\n", time_taken);
                break;
            case 2:
                start = clock();
                fractionalKnapsack(items, n, capacity);
                end = clock();
                time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("Time taken: %.6f seconds\n", time_taken);
                break;
            case 3:
                start = clock();
                branchAndBoundKnapsack(items, n, capacity);
                end = clock();
                time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("Time taken: %.6f seconds\n", time_taken);
                break;
            case 4:
                compareAll(items, n, capacity);
                break;
            case 5:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
