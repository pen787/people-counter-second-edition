Import("env")
import os

def generate_secrets_header(env_file=".env", output_file="include/secrets.h", outputcpp="src/secrets.cpp"):
    # Create output directory if it doesn't exist
    os.makedirs(os.path.dirname(output_file), exist_ok=True)
    
    with open(output_file, "w") as header:
        header.write("// Auto-generated secrets header\n\n")
        header.write("#ifndef SECRETS_H\n#define SECRETS_H\n\n")
        
        try:
            with open(env_file) as f:
                for line in f:
                    line = line.strip()
                    if not line or line.startswith("#"):
                        continue
                    
                    if "=" in line:
                        key, value = line.split("=", 1)
                        key = key.strip()
                        value = value.strip().strip('\'"')
                        
                        # Write as string constant instead of macro
                        header.write(f'extern const char* {key};\n')
                        print(f"[ENV] Added: {key}")
        
        except FileNotFoundError:
            print(f"Warning: {env_file} not found")
            # Create empty declarations to prevent compile errors
            header.write("extern const char* WIFI_SSID;\n")
            header.write("extern const char* WIFI_PASSWORD;\n")
            header.write("extern const char* SUPABASE_URL;\n")
            header.write("extern const char* SUPABASE_API;\n")
        
        header.write("\n#endif // SECRETS_H\n")
    
    with open(outputcpp, "w") as cppf:
        cppf.write("// Auto-generated secrets cpp file\n\n")
        cppf.write("#include \"secrets.h\"\n")
        try:
            with open(env_file) as f:
                for line in f:
                    line = line.strip()
                    if not line or line.startswith("#"):
                        continue
                    
                    if "=" in line:
                        key, value = line.split("=", 1)
                        key = key.strip()
                        value = value.strip().strip('\'"')
                        
                        # Write as string constant instead of macro
                        cppf.write(f'const char* {key} = "{value}";\n')
                        print(f"[ENV] Added: {key}")
        
        except FileNotFoundError:
            print(f"Warning: {env_file} not found")
            # Create empty declarations to prevent compile errors
            cppf.write("extern const char* WIFI_SSID = \"\";\n")
            cppf.write("extern const char* WIFI_PASSWORD = \"\";\n")
            cppf.write("extern const char* SUPABASE_URL = \"\";\n")
            cppf.write("extern const char* SUPABASE_API = \"\";\n")

# Generate the header file before build
generate_secrets_header()