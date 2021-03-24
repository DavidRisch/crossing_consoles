import os
import subprocess


class Hpp2plantumlBug(Exception):
    pass


class UmlDirectory:
    def __init__(self, absolute_path: str, relative_path: str):
        self.absolute_path = absolute_path
        self.relative_path = relative_path
        self.all_contained_files = self.get_all_contained_files(absolute_path)
        self.output_name = relative_path.replace('/', '-')

    @classmethod
    def get_all_contained_files(cls, directory: str):
        all_contained_files = []

        for absolute_path, child_directories, child_files in os.walk(directory):
            all_contained_files += [os.path.join(directory, child_file) for child_file in child_files]
            for child_directory in child_directories:
                all_contained_files += cls.get_all_contained_files(os.path.join(directory, child_directory))

        return all_contained_files

    def get_puml_path(self):
        current_directory = os.path.realpath(os.path.dirname(__file__))
        output_directory = os.path.join(current_directory, 'output')
        if not os.path.isdir(output_directory):
            os.mkdir(output_directory)

        return os.path.join(output_directory, '{}.puml'.format(self.output_name))

    def contains_file_by_partial_path(self, partial_path: str):
        return any([partial_path in file_path for file_path in self.all_contained_files])

    def apply_hacks(self):
        """
        Adds some workarounds in special cases to improve the layout of the output image.
        """
        with open(self.get_puml_path(), 'r') as in_file:
            content = in_file.read()

        if all([self.contains_file_by_partial_path(x) for x in
                ['src/communication/byte_layer', 'src/communication/message_layer',
                 'src/communication/connection_layer']]):
            marker = r'''/' Inheritance relationships '/'''

            # order the three namespaces vertically by adding hidden classes
            content = content.replace(marker, marker + '''
communication.communication::message_layer -down-->  communication.A1
communication.A1 -down--> communication.communication::byte_layer
hide communication.A1

communication.communication::connection_layer -down-->  communication.B1
communication.B1 -down--> communication.communication::message_layer
hide communication.B1
            ''')

        with open(self.get_puml_path(), 'w') as out_file:
            out_file.write(content)


def run_hpp2plantuml(uml_directory: UmlDirectory):
    current_directory = os.path.realpath(os.path.dirname(__file__))

    arguments = [
        '-o', uml_directory.get_puml_path(),
        '-t', os.path.join(current_directory, 'plantuml_template.puml'),
    ]

    for child_path in uml_directory.all_contained_files:
        if child_path.endswith('.h'):
            arguments += ['-i', child_path]

    process = subprocess.run(['hpp2plantuml'] + arguments)

    try:
        process.check_returncode()
    except subprocess.CalledProcessError as e:
        # TODO: Temporary workaround for issue in hpp2plantuml:
        # https://github.com/thibaultmarin/hpp2plantuml/issues/13)
        raise Hpp2plantumlBug from e


def run_plant_uml(uml_directory: UmlDirectory):
    current_directory = os.path.realpath(os.path.dirname(__file__))

    jar_path = os.path.join(current_directory, 'bin', 'plantuml.jar')

    environment = os.environ.copy()
    environment['PLANTUML_LIMIT_SIZE'] = str(2 ** 14)

    process = subprocess.run([
        'java',
        '-jar', jar_path,
        '--verbose', uml_directory.get_puml_path()
    ], env=environment)

    process.check_returncode()


def main():
    src_directory = os.path.join(os.path.dirname(__file__), '..', '..', 'src')
    src_directory = os.path.realpath(src_directory)

    for absolute_path, child_directories, child_files in os.walk(src_directory):
        print("Generating UML for directory:", absolute_path)
        relative_path = os.path.relpath(absolute_path, os.path.dirname(src_directory))

        uml_directory = UmlDirectory(absolute_path, relative_path)

        try:
            run_hpp2plantuml(uml_directory)
            uml_directory.apply_hacks()
            run_plant_uml(uml_directory)
        except Hpp2plantumlBug as e:
            # TODO: Temporary workaround for issue in hpp2plantuml:
            # https://github.com/thibaultmarin/hpp2plantuml/issues/13)
            print('caught', e)


if __name__ == '__main__':
    main()
    print('UML done')
