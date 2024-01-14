#include <bits/stdc++.h>

const int N = 20 + 5;
char configFileName[] = "config.txt";
char cmdFileName[] = "cmd.txt";

int a[N][N], b[N][N], c[1 << N], dir, ratio;
char opt[1];
int matrixSize, randValue;
char errFileName[N];
bool logged, boxed;
int width;

// ����� [l, r] �е�һ������
int random(int l, int r) {
    return rand() * rand() % (r - l + 1) + l;
}

int getWidth(int x) {
    int cnt = 0;
    while (x) {
        x /= 10;
        cnt++;
    }
    return cnt;
}

void print_Unboxed() {
    puts("-----------------");
    for (int i = 1; i <= matrixSize; i++) {
        for (int j = 1; j <= matrixSize; j++)
            printf("%3d ", a[i][j]);
        puts("");
    }
    puts("-----------------");
}

void printBetween() {
    printf("+");
    for (int i = 1; i <= matrixSize; i++) {
        for (int j = 1; j <= width; j++)
            printf("-");
        printf("+");
    }
    printf("\n");
}

void print_Boxed() {
    for (int i = 1; i <= matrixSize; i++)
        for (int j = 1; j <= matrixSize; j++)
            width = std::max(width, getWidth(a[i][j]));
    
    char formatter[N];
    std::sprintf(formatter, "%%%dd|", width);
    
    printBetween();
    for (int i = 1; i <= matrixSize; i++) {
        printf("|");
        for (int j = 1; j <= matrixSize; j++) {
            printf(formatter, a[i][j]);
        }
        printf("\n");
        printBetween();
    }
}

// ������� matrixSize*matrixSize ����ĺ���
void print() {
    boxed ? print_Boxed() : print_Unboxed();
    puts("");
}

void fprint_Unboxed() {
    fprintf(stderr, "-----------------\n");
    for (int i = 1; i <= matrixSize; i++) {
        for (int j = 1; j <= matrixSize; j++)
            fprintf(stderr, "%3d ", a[i][j]);
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "-----------------\n");
}

void fprintBetween() {
    fprintf(stderr, "+");
    for (int i = 1; i <= matrixSize; i++) {
        for (int j = 1; j <= width; j++)
            fprintf(stderr, "-");
        fprintf(stderr, "+");
    }
    fprintf(stderr, "\n");
}

void fprint_Boxed() {
    for (int i = 1; i <= matrixSize; i++)
        for (int j = 1; j <= matrixSize; j++)
            width = std::max(width, getWidth(a[i][j]));
    
    char formatter[N];
    std::sprintf(formatter, "%%%dd|", width);
    
    fprintBetween();
    for (int i = 1; i <= matrixSize; i++) {
        fprintf(stderr, "|");
        for (int j = 1; j <= matrixSize; j++) {
            fprintf(stderr, formatter, a[i][j]);
        }
        fprintf(stderr, "\n");
        fprintBetween();
    }
}

void fprint() {
    boxed ? fprint_Boxed() : fprint_Unboxed();
    fprintf(stderr, "\n");
}

// �ж�һ�����Ƿ�Ϊ����
bool isReal(double f) {
    return f == (int) f;
}

// ��ȥ str ������пո�
void removeSpaces(char *str) {
    int count = 0;
    for (int i = 0; str[i]; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[count++] = str[i];
        }
    }
    str[count] = '\0';
}

// ��� str �Ƿ�Ϊ�����ַ���
int getDigit(char *str) {
    int ans = 0;
    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i]))
            return -1;
        else
            ans = ans * 10 + str[i] - '0';
    }
    return ans;
}

// ��ȡ config.txt �ļ�
int readMatrixSizeFromConfig(char *filename) {
    FILE *file = fopen(filename, "r");
    int size = 4;  // Ĭ�ϴ�СΪ4
    int found = 0;  // ���ڱ���Ƿ��ҵ�������������

    if (file != NULL) {
        char line[100];
        char foundLine[100] = "";  // ���ڱ����ҵ���"SIZE = N"��ʽ����
        while (fgets(line, sizeof(line), file)) {
            if (line[0] != '#') {  // ������#��ͷ����
                removeSpaces(line);
                char *pos = strstr(line, "SIZE=");
                if (pos == line) {
                    char *tmp = pos + 5;
                    int x = getDigit(tmp);
                    if (x != -1 && x > 0) {
                        found = 1;
                        size = x;
                    }
                }
            }
        }

        if (!found) {  // ���û���ҵ�������������
            printf("�����ļ���ʽ����\n");
            if (logged)
                fprintf(stderr, "�����ļ���ʽ����\n");
        }
        fclose(file);
    } else {
        printf("�����ļ���ʽ����\n");
        if (logged)
            fprintf(stderr, "�����ļ���ʽ����\n");
    }

    return size;
}

// ��ȡ RAND ֵ
int readRandomValueFromConfig(char *filename) {
    FILE *file = fopen(filename, "r");
    int value = -1;
    int found = 0;  // ���ڱ���Ƿ��ҵ�������������

    if (file != NULL) {
        char line[100];
        char foundLine[100] = "";  // ���ڱ����ҵ���"RAND = R"��ʽ����
        while (fgets(line, sizeof(line), file)) {
            if (line[0] != '#') {  // ������#��ͷ����
                removeSpaces(line);
                char *pos = strstr(line, "RAND=");
                if (pos == line) {
                    char *tmp = pos + 5;
                    int x = getDigit(tmp);
                    if (x != -1 && x >= 0 && x < 100) {
                        found = 1;
                        value = x;
                    }
                }
            }
        }

        if (!found) {  // ���û���ҵ�������������
            printf("�����ļ���ʽ����\n");
            if (logged)
                fprintf(stderr, "�����ļ���ʽ����\n");
        }
        fclose(file);
    } else {
        printf("�����ļ���ʽ����\n");
        if (logged)
            fprintf(stderr, "�����ļ���ʽ����\n");
    }

    return value;
}

// ��ȡ Log ��Ӧ�� FileName
void readLogFileNameFromConfig(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        char line[100];
        while (fgets(line, sizeof(line), file)) {
            if (line[0] != '#') {  // ������#��ͷ����
                removeSpaces(line);
                char *pos = strstr(line, "LOG=");
                if (pos == line) {
                    char *tmp = pos + 4;
                    strcpy(errFileName, tmp);
                    logged = true;
                }
            }
        }
        fclose(file);
    } else {
        printf("�����ļ���ʽ����\n");
        if (logged)
            fprintf(stderr, "�����ļ���ʽ����\n");
    }
}

// ��ȡ Log ��Ӧ�� FileName
void readBoxFromConfig(char *filename) {
    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        char line[100];
        while (fgets(line, sizeof(line), file)) {
            if (line[0] != '#') {  // ������#��ͷ����
                removeSpaces(line);
                if (strcmp(line, "BOX=TRUE"))
                    boxed = true;
            }
        }
        fclose(file);
    } else {
        printf("�����ļ���ʽ����\n");
        if (logged)
            fprintf(stderr, "�����ļ���ʽ����\n");
    }
}

// ����ð�ݲ���
void bubble() {
    for (int j = 1; j <= matrixSize; j++) { // ö����
        for (int i = matrixSize - 1; i >= 1; i--) { // ö����
            for (int k = i; k <= matrixSize - 1; ++k) { // ���ϵ��£��滻 0
                if (a[k + 1][j] == 0) // �����һλΪ 0
                    std::swap(a[k][j], a[k + 1][j]); // �����������ڡ�ð�ݡ�����
            }
        }
    }
}

// �������ڲ���
void calc() {
    for (int j = 1; j <= matrixSize; j++) {
        for (int i = 1; i <= matrixSize - 1; ) {
            if (a[i][j] == a[i + 1][j]) {
                a[i + 1][j] *= 2;
                a[i][j] = 0;
                i += 2;
            } else
                i++;
        }
    }
}

// ����������� 0 ��λ�ã��� R% �ı����������޸�Ϊ 2 �� 4
void modify() {
    int cnt = 0;
    for (int i = 1; i <= matrixSize; i++)
        for (int j = 1; j <= matrixSize; j++)
            cnt += a[i][j] == 0; // �ȼ����ж��ٸ� 0

    int num = cnt * randValue / 100;
    int arr[N * N] = { 0 };
    for (int i = 1; i <= num; i++) // �� num ��λ�ã���Ҫ�����޸�
        arr[i] = 1;
   	// arr[i] == 0 ��ʾ�� i �� 0 ����Ҫ�޸�
    // arr[i] == 1 ��ʾ�� i �� 0 ��Ҫ�޸�
    std::random_shuffle(arr + 1, arr + cnt + 1); // ��������� R% ��

    cnt = 0;
    for (int i = 1; i <= matrixSize; i++) {
        for (int j = 1; j <= matrixSize; j++) {
            cnt += a[i][j] == 0;
            if (a[i][j] == 0 && arr[cnt]) { // �����ǰλ����Ҫ�޸�
                a[i][j] = random(0, 1) ? 2 : 4; // �޸�Ϊ 2 �� 4
                return;
            }
        }
    }
}

// �ж�һ�������Ƿ�Ϸ� / �����Ƿ�Ϊ *
bool valid(int x, int y) {
    return x >= 1 && x <= matrixSize && y >= 1 && y <= matrixSize && a[x][y] != 0;
}

void printStar() {
    for (int i = 1; i <= matrixSize; i++) {
        for (int j = 1; j <= matrixSize; j++) {
            if (a[i][j]) {
                putchar('*');
            } else {
                int star = 0;
                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        star += valid(i + k, j + l);
                    }
                }
                printf("%d", star);
            }
        }
        puts("");
    }
    puts("");
}

void fprintStar() {
    for (int i = 1; i <= matrixSize; i++) {
        for (int j = 1; j <= matrixSize; j++) {
            if (a[i][j]) {
                fprintf(stderr, "*");
            } else {
                int star = 0;
                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        star += valid(i + k, j + l);
                    }
                }
                fprintf(stderr, "%d", star);
            }
        }
        fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}

void operate(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        freopen("cmd.txt", "r", stdin);
        print();
        if (logged)
            fprint();
        while (scanf("%s", opt) != EOF) {
            if (opt[0] == 'C') { // C ��ʾ Compress
                scanf("%d", &dir);
                switch (dir) {
                    case 0: // ����
                        for (int j = 1; j <= matrixSize; j++) { // ö����
                            int sum = 0;
                            for (int i = 1; i <= matrixSize; i++) { // ö���У����
                                sum += a[i][j];
                                a[i][j] = 0; // ���൥Ԫ��Ϊ 0
                            }
                            a[1][j] = sum;
                        }
                        break;
                    case 1: // ����
                        for (int i = 1; i <= matrixSize; i++) { // ö����
                            int sum = 0;
                            for (int j = 1; j <= matrixSize; j++) { // ö���У����
                                sum += a[i][j];
                                a[i][j] = 0; // ���൥Ԫ��Ϊ 0
                            }
                            a[i][1] = sum;
                        }
                        break;
                    case 2: // ����
                        for (int j = 1; j <= matrixSize; j++) { // ö����
                            int sum = 0;
                            for (int i = 1; i <= matrixSize; i++) { // ö���У����
                                sum += a[i][j];
                                a[i][j] = 0; // ���൥Ԫ��Ϊ 0
                            }
                            a[matrixSize][j] = sum;
                        }
                        break;
                    case 3: // ����
                        for (int i = 1; i <= matrixSize; i++) { // ö����
                            int sum = 0;
                            for (int j = 1; j <= matrixSize; j++) { // ö���У����
                                sum += a[i][j];
                                a[i][j] = 0; // ���൥Ԫ��Ϊ 0
                            }
                            a[i][matrixSize] = sum;
                        }
                        break;
                    default:
                        continue;
                }
            } else if (opt[0] == 'R') { // R ��ʾ Rotate
                scanf("%d", &dir);
                for (int i = 1; i <= matrixSize; i++)
                    for (int j = 1; j <= matrixSize; j++)
                        b[i][j] = a[matrixSize + 1 - j][i]; // ������ת 90 �ȣ����Ƶ� b ������
                for (int i = 1; i <= matrixSize; i++)
                    for (int j = 1; j <= matrixSize; j++)
                        a[i][j] = b[i][j]; // �������ƶ��� a ����
            } else if (opt[0] == 'M') { // M ��ʾ Melt
                bubble();
                calc();
                bubble();
                modify();
            } else if (opt[0] == 'Q') { // Q ��ʾ Quit
                break;
            }
            print();
            if (logged)
                fprint();
        }
    } else {
        printStar();
        if (logged)
            fprintStar();
    }
}

int main() {
    
    srand(time(0));
    matrixSize = readMatrixSizeFromConfig(configFileName);
    randValue = readRandomValueFromConfig(configFileName);
    readLogFileNameFromConfig(configFileName);
    if (logged)
        freopen(errFileName, "w", stderr);
    readBoxFromConfig(configFileName);

    // printf("matrixSize = %d\nrandValue = %d\n", matrixSize, randValue);

    // �������
    if (randValue == -1 || randValue == 0) {
        c[1] = 1 << random(1, 2);
    } else {
        for (int i = 1; i <= matrixSize * matrixSize * randValue / 100.0; i++) // һ����Ϊ 2 ����Ȼ�����ݣ�����Ϊ 0
            c[i] = 1 << random(1, 4);
    }

    std::random_shuffle(c + 1, c + matrixSize * matrixSize + 1); // ����
    for (int i = 1; i <= matrixSize; i++)
        for (int j = 1; j <= matrixSize; j++)
            a[i][j] = c[(i - 1) * matrixSize + j]; // �ѳ���Ϊ matrixSize^2 �� c ��������ɾ���

    // ���� cmd.txt �еĲ���
    operate(cmdFileName);

    return 0;
}