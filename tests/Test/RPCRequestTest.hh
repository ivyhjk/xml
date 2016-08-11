<?hh // strict

namespace Ivyhjk\Test\Xml;

use Exception;
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

        while(list($_, $node) = each($method)) {
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

        $this->assertSame('f00', $value);
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

        $this->assertSame(133.7, $value);
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

        while(list($_, $node) = each($method)) {
            $this->assertSame('MyMethod', (string) $node);
        }

        $keys = $xml->xpath('/methodCall/params/param/value/struct/member/name');

        while(list($_, $node) = each($keys)) {
            $this->assertContains((string) $node, ['foo', 'bar', 'baz']);
        }

        $nestedKeys = $xml->xpath('/methodCall/params/param/value/struct/member/value/struct/member/name');

        while(list($_, $node) = each($nestedKeys)) {
            $this->assertContains((string) $node, ['zzz', 'ccc']);
        }

        $reallyNestedKeys = $xml->xpath('/methodCall/params/param/value/struct/member/value/struct/member/value/struct/member/value/struct/member/name');

        while(list($_, $node) = each($reallyNestedKeys)) {
            $this->assertSame('bbb', (string) $node);
        }
    }

    /**
     * Test the decode method when is sent an invalid xml.
     *
     * @return void
     */
    public function testDecodeInvalidXml() : void
    {
        try {
            RPCRequest::decode('');
        } catch (Exception $e) {
            $this->assertInstanceOf(XmlException::class, $e);
        }
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

        $this->assertEquals($expected, $decoded);
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

        $this->assertEquals($expected, $decoded);
    }
}
