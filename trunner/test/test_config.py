from pathlib import Path

import pytest

import trunner.config
from trunner.config import Config, ConfigParser, TestConfig, TestCaseConfig, ParserError, \
    array_value, ALL_TARGETS, DEFAULT_TARGETS

# Pytest tries to collect come classes as tests, mark them as not testable
TestConfig.__test__ = False
TestCaseConfig.__test__ = False


def sort_targets(config):
    targets = config.get('targets')
    if not targets:
        return

    for value in targets.values():
        value.sort()


@pytest.mark.parametrize('case, ans', [
    ({'value': [1, 2, 3], 'include': [1, 4], 'exclude': [2]},
     [1, 3, 4]),
    ({'value': [], 'include': [1], 'exclude': [1]},
     []),
    ({'value': [1], 'include': [0], 'exclude': [1]},
     [0]),
    ({},
     []),
    ({'value': []},
     []),
])
def test_array_value(case, ans):
    assert array_value(case) == ans


class Test_ConfigParser:
    @pytest.fixture
    def parser(self):
        return ConfigParser()

    def test_keywords_exc(self, parser):
        test = TestConfig({'not known keyword': 'for real'})
        with pytest.raises(ParserError):
            parser.parse_keywords(test)

    @pytest.mark.parametrize('case, ans', [
        ({}, None),
        ({'harness': 'harness.py'}, Path('harness.py')),
    ])
    def test_harness_keyword(self, parser, tmp_path, case, ans):
        test = TestConfig(case)
        parser.parse_harness(test)
        harness = test.get('harness')
        assert harness == ans
        assert not harness or test['type'] == 'harness'

    @pytest.mark.parametrize('case', [
        {'harness': 'harness.exe'},
    ])
    def test_harness_keyword_exc(self, parser, case):
        test = TestConfig(case)
        with pytest.raises(ParserError):
            parser.parse_harness(test)

    @pytest.mark.parametrize('case, ans', [
        ({'type': type}, type) for type in ConfigParser.TEST_TYPES] + [({}, None)]
    )
    def test_type_keyword(self, parser, case, ans):
        test = TestConfig(case)
        parser.parse_type(test)
        assert test.get('type') == ans

    @pytest.mark.parametrize('case', [
        {'type': 'type_that_do_not_exist'},
    ])
    def test_type_keyword_exc(self, parser, case):
        test = TestConfig(case)
        with pytest.raises(ParserError):
            parser.parse_type(test)

    @pytest.mark.parametrize('case, ans', [
        ({}, None),
        ({'ignore': True}, True),
    ])
    def test_ignore_keyword(self, parser, case, ans):
        test = TestConfig(case)
        parser.parse_ignore(test)
        assert test.get('ignore') == ans

    @pytest.mark.parametrize('case', [
        {'ignore': 'false'},
        {'ignore': 1},
        {'ignore': 'yes'},
    ])
    def test_ignore_keyword_exc(self, parser, case):
        test = TestConfig(case)
        with pytest.raises(ParserError):
            parser.parse_ignore(test)

    @pytest.mark.parametrize('case, ans', [
        ({}, None),
        ({'timeout': 1}, 1),
        ({'timeout': '10'}, 10),
        ({'timeout': 0}, 0),
    ])
    def test_timeout_keyword(self, parser, case, ans):
        test = TestConfig(case)
        parser.parse_timeout(test)
        assert test.get('timeout') == ans

    @pytest.mark.parametrize('case', [
        {'timeout': '0x10'},
        {'timeout': 'ten seconds'},
    ])
    def test_timeout_keyword_exc(self, parser, case):
        test = TestConfig(case)
        with pytest.raises(ParserError):
            parser.parse_timeout(test)

    @pytest.mark.parametrize('case', [
        {'value': [], 'include': [], 'exclude': []},
        {'include': [], 'exclude': []},
        {'value': []},
    ])
    def test_is_array(self, parser, case):
        assert ConfigParser.is_array(case)

    @pytest.mark.parametrize('case', [
        {'value': [], 'include': [], 'exclude': [], 'maybe_also_this': []},
        {'not_array_keyword': []},
    ])
    def test_is_array_exc(self, parser, case):
        with pytest.raises(ParserError):
            ConfigParser.is_array(case)

    @pytest.mark.parametrize('case, ans', [
        ({}, None),
        ({'targets': {'value': ['ia32-generic']}},
         {'value': ['ia32-generic']}),
    ])
    def test_targets_keyword(self, parser, case, ans):
        test = TestConfig(case)
        parser.parse_targets(test)
        assert test.get('targets') == ans

    @pytest.mark.parametrize('case', [
        {'targets': {'value': ['invalid-target']}},
        {'targets': 'ia32-generic'},
    ])
    def test_targets_keyword_exc(self, parser, case):
        test = TestConfig(case)
        with pytest.raises(ParserError):
            parser.parse_targets(test)

    @pytest.mark.parametrize('case', [
        {'harness': Path('harness.py')},
    ])
    def test_resolve_harness(self, parser, tmp_path, case):
        test = TestConfig(case)
        path = tmp_path / test['harness']
        path.touch()
        parser.resolve_harness(test, tmp_path)
        assert test['harness'] == path

    @pytest.mark.parametrize('case', [
        {},
        {'harness': Path('harness.py')},
    ])
    def test_resolve_harness_exc(self, parser, tmp_path, case):
        test = TestConfig(case)
        with pytest.raises(ParserError):
            parser.resolve_harness(test, tmp_path)

    @pytest.fixture
    def fixed_path(self):
        saved_path = trunner.config.PHRTOS_PROJECT_DIR
        trunner.config.PHRTOS_PROJECT_DIR = Path('/home/user/phoenix-rtos-project')
        yield
        trunner.config.PHRTOS_PROJECT_DIR = saved_path

    @pytest.mark.parametrize('path, case, ans', [
        ('/home/user/phoenix-rtos-project/phoenix-rtos-tests/example',
         {'name': 'name1'},
         'phoenix-rtos-tests.example.name1'),
        ('path/to/test',
         {'name': 'name2'},
         'path.to.test.name2'),
    ])
    @pytest.mark.usefixtures('fixed_path')
    def test_resolve_name(self, parser, path, case, ans):
        test = TestConfig(case)
        parser.resolve_name(test, path)
        assert test.get('name') == ans

    @pytest.mark.parametrize('case', [
        {},
    ])
    def test_resolve_name_exc(self, parser, tmp_path, case):
        test = TestConfig(case)
        with pytest.raises(ParserError):
            parser.resolve_name(test, tmp_path)

    @pytest.mark.parametrize('targets, case, ans', [
        (
            ['target0', 'target2', 'target5'],
            {'targets': {
                'value': ['target0', 'target1', 'target2', 'target3'],
                'include': ['target4'],
                'exclude': ['target1']
            }},
            {'targets': {
                'value': ['target0', 'target2']
            }}
        ),
        (
            ['target2'],
            {'targets': {
                'value': ['target1']
            }},
            {'targets': {
                'value': []
            }}
        ),
    ])
    def test_resolve_targets(self, parser, targets, case, ans):
        test = TestConfig(case)
        parser.resolve_targets(test, targets)

        for config in test, ans:
            sort_targets(config)

        assert test == ans


class Test_Config:
    def test_is_dict(self):
        # Test if Config inherit from the dict
        # Most of the code is based on this inheritance
        test = Config(dict())
        assert isinstance(test, dict)

    @pytest.mark.parametrize('main, minor, ans', [
        (
            {'targets': {
                'value': ['target1'],
                'include': ['target2'],
                'exclude': ['target3']
            }},
            {},
            {'targets': {
                'value': ['target1'],
                'include': ['target2'],
                'exclude': ['target3']
            }},
        ),
        (
            {'targets': {
                'value': ['target1'],
                'include': ['target2'],
                'exclude': ['target3']
            }},
            {'targets': {
                'value': ['target4'],
            }},
            {'targets': {
                'value': ['target1'],
                'include': ['target2'],
                'exclude': ['target3']
            }},
        ),
        (
            {},
            {'targets': {
                'value': ['target1'],
                'include': ['target2'],
                'exclude': ['target3']
            }},
            {'targets': {
                'value': ['target1'],
                'include': ['target2'],
                'exclude': ['target3']
            }},
        ),
        (
            {'targets': {
                'include': ['target2']
            }},
            {'targets': {
                'include': ['target1'],
            }},
            {'targets': {
                'value': list(ALL_TARGETS) + ['target1'],
                'include': ['target2']
            }}
        ),
    ])
    def test_join_targets(self, main, minor, ans):
        main, minor = Config(main), TestConfig(minor)
        main.join_targets(minor)

        # Sort values to assure equality
        for config in main, ans:
            sort_targets(config)

        assert main == ans

    @pytest.mark.parametrize('main, minor, ans', [
        (
            {'key1': 'value1'},
            {'key2': 'value2'},
            {'key1': 'value1', 'key2': 'value2'}
        ),
        (
            {'key1': 'value1'},
            {'key1': 'value2'},
            {'key1': 'value1'}
        ),
    ])
    def test_join(self, main, minor, ans):
        main, minor = Config(main), TestConfig(minor)
        main.join(minor)
        assert main == ans

    @pytest.mark.parametrize('case, ans', [
        ({}, {'targets': {'value': DEFAULT_TARGETS}}),
        ({'targets': {}}, {'targets': {'value': DEFAULT_TARGETS}}),
        ({'targets': {'value': ['my-target']}}, {'targets': {'value': ['my-target']}}),
    ])
    def test_setdefault_targets(self, case, ans):
        test = TestConfig(case)
        test.setdefault_targets()
        assert test == ans


class Test_TestConfig:
    def test_copy_per_target(self):
        targets_value = ['target1', 'target2']
        name = 'test'
        exec_bin = 'some_binary'

        test = TestConfig({
            'name': name,
            'exec': exec_bin,
            'targets': {'value': targets_value}
        })

        answers = []
        for target in targets_value:
            answers.append(TestConfig({
                'name': name,
                'exec': exec_bin,
                'target': target
            }))

        tests = test.copy_per_target()
        for test in tests:
            assert test in answers


class Test_TestCaseConfig:
    def test_extract_components(self):
        config_tests = [{'name': 'test1'}, {'name': 'test2'}]
        config = {
            'test': {
                'here is': 'main config',
                'tests': config_tests
            }
        }

        main, tests = TestCaseConfig.extract_components(config)
        assert main == {'here is': 'main config'}
        assert tests == config_tests
