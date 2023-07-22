# MaconomyApp
Application for importing a time log and uploading the result to Maconomy.

## Building
This project can either be opened in Visual Studio or build using make.

### Prerequisites
The application is dependent on cURL (https://curl.se/download.html).

To build with make you need to point to the cURL directory containing the headers and library files:
```
make CURL_PATH=<path to cURL directory>
Example: make CURL_PATH=C:/Users/Admin/CURL
```
