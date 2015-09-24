#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

int main() {
	int num_servers, num_adns, num_clients;
	double load;
  cerr << "<num_servers> <num_adns> <num_clients> <load_frac>" << endl;
	cin >> num_servers >> num_adns >> num_clients >> load;

	// The server capacities in requests per second
	vector<double> server_caps(num_servers, 0);
	// The load on each ADNS in terms of requests per second
	vector<double> load_on_adns(num_adns, 0);
	// global load as a fraction of total server capacity
	double total_load;
	
  vector<vector<double> > adns_from_client(num_adns, vector<double>(num_clients, 0));
	vector<vector<double> > cost_client_server(num_clients, vector<double>(num_servers, 0));

	{
		double total_cap = 0;
		for (int i = 0; i < num_servers; ++i) {
			server_caps[i] = (rand() % 100) + 50;
			total_cap += server_caps[i];
		}
		total_load = total_cap * load;
	}

	{
		double total_load_tmp = 0;
		for (int i = 0; i < num_adns; ++i) {
			load_on_adns[i] = (rand() % 100) + 50;
			total_load_tmp += load_on_adns[i];
		}
		for (int i = 0; i < num_adns; ++i) {
			load_on_adns[i] /= total_load_tmp;
		}
	}

	for (int i = 0; i < num_adns; ++i) {
		double tmp_total = 0;
		for (int j = 0; j < num_clients; ++j) {
			adns_from_client[i][j] = rand() % 100;
			// Makes ~ 60% client loads 0.
			if (adns_from_client[i][j] < 60) {
				adns_from_client[i][j] = 0;
			}
			tmp_total += adns_from_client[i][j];
		}
		for (int j = 0; j < num_clients; ++j) {
			adns_from_client[i][j] /= tmp_total;
		}
	}

	{
		int modulus_val = min(num_clients, num_servers);
		for (int i = 0; i < num_clients; ++i) {
			for (int j = 0; j < num_servers; ++j) {
				cost_client_server[i][j] = abs((i % modulus_val) - (j % modulus_val));
			}
		}
	}

	cout << "Minimize" << endl;
	cout << "obj: ";
	for (int i = 0; i < num_clients; i++) {
		for (int j = 0; j < num_servers; j++) {
			for (int k = 0; k < num_adns; k++) {
				double coeff = load_on_adns[k] * adns_from_client[k][i] * cost_client_server[i][j];
				cout << coeff << " " << "f_" << i << "_" << j << "_" << k << " ";
				if (k != num_adns - 1 || j != num_servers - 1 || i != num_clients - 1) {
					cout << "+ ";
				}
			}
		}
	}
	cout << endl;
	cout << "Subject To" << endl;
	
	// Add the server load constraints
	int cnum = 0;
	for (int j = 0; j < num_servers; ++j) {
		cout << "c" << cnum++ << ": ";
		for (int i = 0; i < num_clients; ++i) {
			for (int k = 0; k < num_adns; ++k) {
				double coeff = load_on_adns[k] * adns_from_client[k][i];
			  cout << coeff << " f_" << i << "_" << j << "_" << k << " ";
				if (k != num_adns - 1 || i != num_clients - 1) {
					cout << "+ ";
				}
			}
		}
		cout << " <= " << server_caps[j] / (float) total_load << endl;
	}
	// Add the ADNS routing sum constraint
	for (int i = 0; i < num_clients; ++i) {
		for (int k = 0; k < num_adns; ++k) {
			cout << "c" << cnum++ << ": ";
			for (int j = 0; j < num_servers; ++j) {
			  cout << "f_" << i << "_" << j << "_" << k << " ";
				if (j != num_servers - 1) {
					cout << "+ ";
				}
			}
			cout << "= 1" << endl;
		}
	}
  cout << "Bounds" << endl;
	for (int i = 0; i < num_clients; ++i) {
		for (int j = 0; j < num_servers; ++j) {
			for (int k = 0; k < num_adns; ++k) {
				cout << "0 <= f_" << i << "_" << j << "_" << k << " <= 1" << endl;
			}
		}
	}
	cout << "End" << endl;

}




			




