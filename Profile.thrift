/** 
  Thrift file define services for both Client & Server
  Project Profile Demo
*/

// data structure for storing user data
struct  Profile {
	1: i32 _id, //auto
	2: string name, //input
	3: bool isMale, //input
	4: i32 birthDate, //input - int for storage
	5: i32 lastActiveTime //auto
	6: i32 registerTime // auto
}

// services exposed by server
// define input data structure
union dateInput {
	1: string dateString,
	2: i32 dateInt
}
struct InputProfileData {
	1: string name,
	2: bool isMale,
	3: dateInput birthDate // input can be int or string
}

// define error code as enumerate list
enum ErrorCode {
	SUCCESS = 0,
	USER_NOT_FOUND,
	INVALID_PARAMETERS,
	INTERNAL_ERROR 
}

// define return data structure
struct GetProfileResult {
	// return code
	1: required ErrorCode code, 
	2: Profile data
}

service UserProfile {
	i32 CreateUser(1: InputProfileData profile),
	GetProfileResult GetUser(1: i32 _id)
	ErrorCode UpdateUser (1: i32 _id, 2: InputProfileData updateData),
	ErrorCode RemoveUser (1: i32 _id)
}