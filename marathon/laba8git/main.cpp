#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Participant {
    int number;
    string last_name;
    string first_name;
    string patronymic;
    int start_seconds;
    int finish_seconds;
    string club;

    int time_result() const {
        return finish_seconds - start_seconds;
    }
};

int time_to_seconds(const string& time_str) {
    int hours, minutes, seconds;
    char delimiter;
    istringstream iss(time_str);
    iss >> hours >> delimiter >> minutes >> delimiter >> seconds;
    return hours * 3600 + minutes * 60 + seconds;
}

vector<Participant> read_participants(const string& filename) {
    vector<Participant> participants;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        Participant p;
        string start_time, finish_time;
        string club_part;

        iss >> p.number >> p.last_name >> p.first_name >> p.patronymic >> start_time >> finish_time;
        getline(iss, club_part);
        p.club = club_part.substr(club_part.find_first_not_of(' '));
        p.start_seconds = time_to_seconds(start_time);
        p.finish_seconds = time_to_seconds(finish_time);

        participants.push_back(p);
    }

    return participants;
}

vector<Participant> filter_spartak(const vector<Participant>& participants) {
    vector<Participant> result;
    for (const auto& p : participants) {
        if (p.club == "Спартак") {
            result.push_back(p);
        }
    }
    return result;
}

vector<Participant> filter_better_25000(const vector<Participant>& participants) {
    vector<Participant> result;
    for (const auto& p : participants) {
        if (p.time_result() < 25000) {
            result.push_back(p);
        }
    }
    return result;
}

void bubble_sort(vector<Participant>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j].time_result() > arr[j + 1].time_result()) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void quick_sort(vector<Participant>& arr, int left, int right) {
    if (left >= right) return;
    Participant pivot = arr[(left + right) / 2];
    int i = left, j = right;

    while (i <= j) {
        while (arr[i].club < pivot.club || (arr[i].club == pivot.club && arr[i].last_name < pivot.last_name)) i++;
        while (arr[j].club > pivot.club || (arr[j].club == pivot.club && arr[j].last_name > pivot.last_name)) j--;
        if (i <= j) swap(arr[i++], arr[j--]);
    }

    quick_sort(arr, left, j);
    quick_sort(arr, i, right);
}

int main() {
    cout << "Вариант 1: Результаты марафона" << endl;
    cout << "Автор: Якубовский Дима " << endl;
    auto participants = read_participants("input.txt");

    auto spartak = filter_spartak(participants);
    auto best_results = filter_better_25000(participants);

    auto bubble_sorted = participants;
    bubble_sort(bubble_sorted);

    auto quick_sorted = participants;
    quick_sort(quick_sorted, 0, quick_sorted.size() - 1);

    cout << "Участники Спартака:" << endl;
    for (const auto& p : spartak) {
        cout << p.last_name << " " << p.time_result() << endl;
    }

    return 0;
}
