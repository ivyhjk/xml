<?hh // strict

namespace Ivyhjk\Test\Xml;

use SimpleXMLElement;
use Ivyhjk\Xml\RPCRequest;
use Ivyhjk\Xml\Exception\XmlException;

/**
 * Test xml test workflow.
 *
 * @since v1.0.0
 * @version v1.0.0
 * @package Ivyhjk\Test\Xml
 * @author Elvis Munoz <elvis.munoz.f@gmail.com>
 * @copyright Copyright (c) 2016, Elvis Munoz
 * @license https://opensource.org/licenses/MIT MIT License
 */
/* HH_FIXME[4123] */ /* HH_FIXME[2049] */
class RPCRequestTest extends \PHPUnit_Framework_TestCase
{
    /**
     * Get a single value.
     *
     * @param string $XMLtext
     * @param string $path
     *
     * @return mixed
     */
    private function getSingleValue(string $XMLtext, string $path) : mixed
    {
        $xml = new SimpleXMLElement($XMLtext);

        $method = $xml->xpath($path);

        foreach ($method as $node) {
            $value = (string) $node;

            if (is_numeric($value)) {
                if ((int) $value != $value) {
                    return (float) $value;
                } else {
                    return (int) $value;
                }
            }

            return $value;
        }
    }

    /**
     * Test the encode request with siple.
     *
     * @return void
     */
    public function testEncodeRequestString() : void
    {
        $response = RPCRequest::encode('MyMethod', 'f00');

        $value = $this->getSingleValue($response, '/methodCall/params/param/value/string');

        static::assertSame('f00', $value);
    }

    /**
     * Test the encode request with siple.
     *
     * @return void
     */
    public function testEncodeRequestDouble() : void
    {
        $response = RPCRequest::encode('MyMethod', 133.7);

        $value = $this->getSingleValue($response, '/methodCall/params/param/value/double');

        static::assertSame(133.7, $value);
    }

    /**
     * Test the encode request with nested values.
     *
     * @return void
     */
    public function testEncodeRequestNestedValues() : void
    {
        $response = RPCRequest::encode('MyMethod', [
            'foo' => 'bar',
            'bar' => 'baz',
            'baz' => [
                'zzz' => 'xxx',
                'ccc' => [
                    'vvv' => [
                        'bbb' => 'nnn'
                    ]
                ]
            ]
        ]);

        $xml = new SimpleXMLElement($response);

        $method = $xml->xpath('/methodCall/methodName');

        foreach ($method as $node) {
            static::assertSame('MyMethod', (string) $node);
        }

        $keys = $xml->xpath('/methodCall/params/param/value/struct/member/name');

        foreach ($keys as $node) {
            static::assertContains((string) $node, ['foo', 'bar', 'baz']);
        }

        $nestedKeys = $xml->xpath('/methodCall/params/param/value/struct/member/value/struct/member/name');

        foreach ($nestedKeys as $node) {
            static::assertContains((string) $node, ['zzz', 'ccc']);
        }

        $reallyNestedKeys = $xml->xpath('/methodCall/params/param/value/struct/member/value/struct/member/value/struct/member/value/struct/member/name');

        foreach ($reallyNestedKeys as $node) {
            static::assertSame('bbb', (string) $node);
        }
    }

    /**
     * Test the decode method when is sent an invalid xml.
     *
     * @return void
     */
    public function testDecodeInvalidXml() : void
    {
        $this->expectException(XmlException::class);
        RPCRequest::decode('');
    }

    /**
     * Test decode method when is sent a string.
     *
     * @return void
     */
    public function testDecodeBasicString() : void
    {
        $xml = '
        <methodCall>
            <methodName>MyMethod</methodName>
            <params>
                <param>
                    <value>
                        <string>aa</string>
                    </value>
                </param>
                <param>
                    <value>
                        <string>bb</string>
                    </value>
                </param>
                <param>
                    <value>
                        <string>cc</string>
                    </value>
                </param>
            </params>
        </methodCall>
        ';

        $decoded = RPCRequest::decode($xml);

        $expected = Map{
            'method' => 'MyMethod',
            'parameters' => Vector{
                'aa',
                'bb',
                'cc'
            }
        };

        static::assertEquals($expected, $decoded);
    }

    /**
     * Test the decoded method with nested parameters.
     *
     * @return void
     */
    public function testDecodeDestedParameters() : void
    {
        $xml = '
        <methodCall>
            <methodName>MyMethod</methodName>
            <params>
                <param>
                    <value>
                        <struct>
                            <member>
                                <name>foo</name>
                                <value>
                                    <string>bar</string>
                                </value>
                            </member>
                            <member>
                                <name>bar</name>
                                <value>
                                    <string>baz</string>
                                </value>
                            </member>
                            <member>
                                <name>baz</name>
                                <value>
                                    <struct>
                                        <member>
                                            <name>zzz</name>
                                            <value>
                                                <string>xxx</string>
                                            </value>
                                        </member>
                                        <member>
                                            <name>ccc</name>
                                            <value>
                                                <struct>
                                                    <member>
                                                        <name>vvv</name>
                                                        <value>
                                                            <struct>
                                                                <member>
                                                                    <name>bbb</name>
                                                                    <value>
                                                                        <string>nnn</string>
                                                                    </value>
                                                                </member>
                                                            </struct>
                                                        </value>
                                                    </member>
                                                </struct>
                                            </value>
                                        </member>
                                    </struct>
                                </value>
                            </member>
                        </struct>
                    </value>
                </param>
            </params>
        </methodCall>
        ';

        $decoded = RPCRequest::decode($xml);

        $expected = Map{
            'method' => 'MyMethod',
            'parameters' => Vector{
                Map{
                    'foo' => 'bar',
                    'bar' => 'baz',
                    'baz' => Map{
                        'zzz' => 'xxx',
                        'ccc' => Map{
                            'vvv' => Map{
                                'bbb' => 'nnn'
                            }
                        }
                    }
                }
            }
        };

        static::assertEquals($expected, $decoded);
    }

    /**
     * Test encode method when an array is given.
     *
     * @return void
     */
    public function testArrayEncode() : void
    {
        $toEncode = ['bar', 'baz'];

        $expected = preg_replace(['/>\s+</', '/\n/', '/\s+</'], ['><', '', '<'],'
            <?xml version="1.0" encoding="iso-8859-1"?>
            <methodCall>
                <methodName>foo</methodName>
                <params>
                    <param>
                        <value>
                            <string>bar</string>
                        </value>
                    </param>
                    <param>
                        <value>
                            <string>baz</string>
                        </value>
                    </param>
                </params>
            </methodCall>');

        $encoded = preg_replace('/\n/', '', RPCRequest::encode('foo', $toEncode));

        static::assertEquals($expected, $encoded);
    }
}
