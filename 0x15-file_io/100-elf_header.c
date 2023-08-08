#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void validate_elf_format(unsigned char *magic_numbers);
void print_magic_numbers(unsigned char *magic_numbers);
void print_class(unsigned char *magic_numbers);
void print_data_encoding(unsigned char *magic_numbers);
void print_version(unsigned char *magic_numbers);
void print_abi(unsigned char *magic_numbers);
void print_osabi(unsigned char *magic_numbers);
void print_type(unsigned int elf_type, unsigned char *magic_numbers);
void print_entry_point(unsigned long int entry_point, unsigned char *magic_numbers);
void close_elf_file(int elf_file);

/**
 * validate_elf_format - Checks if a file is an ELF file.
 * @magic_numbers: A pointer to an array containing the ELF magic numbers.
 *
 * Description: If the file is not an ELF file - exit code 98.
 */
void validate_elf_format(unsigned char *magic_numbers)
{
	int index;

	for (index = 0; index < 4; index++)
	{
		if (magic_numbers[index] != 127 &&
		    magic_numbers[index] != 'E' &&
		    magic_numbers[index] != 'L' &&
		    magic_numbers[index] != 'F')
		{
			dprintf(STDERR_FILENO, "Error: Not an ELF file\n");
			exit(98);
		}
	}
}

// ... (Rest of the functions remain unchanged)

/**
 * main - Displays the information contained in the
 *        ELF header at the start of an ELF file.
 * @argc: The number of arguments supplied to the program.
 * @argv: An array of pointers to the arguments.
 *
 * Return: 0 on success.
 *
 * Description: If the file is not an ELF File or
 *              the function fails - exit code 98.
 */
int main(int __attribute__((__unused__)) argc, char *argv[])
{
	Elf64_Ehdr *header;
	int elf_file, read_result;

	elf_file = open(argv[1], O_RDONLY);
	if (elf_file == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
		exit(98);
	}

	header = malloc(sizeof(Elf64_Ehdr));
	if (header == NULL)
	{
		close_elf_file(elf_file);
		dprintf(STDERR_FILENO, "Error: Can't read file %s\n", argv[1]);
		exit(98);
	}

	read_result = read(elf_file, header, sizeof(Elf64_Ehdr));
	if (read_result == -1)
	{
		free(header);
		close_elf_file(elf_file);
		dprintf(STDERR_FILENO, "Error: `%s`: No such file\n", argv[1]);
		exit(98);
	}

	validate_elf_format(header->e_ident);
	printf("ELF Header:\n");
	print_magic_numbers(header->e_ident);
	print_class(header->e_ident);
	print_data_encoding(header->e_ident);
	print_version(header->e_ident);
	print_osabi(header->e_ident);
	print_abi(header->e_ident);
	print_type(header->e_type, header->e_ident);
	print_entry_point(header->e_entry, header->e_ident);

	free(header);
	close_elf_file(elf_file);
	return (0);
}
